#ifndef UTF8Iterator_H
#define UTF8Iterator_H

#include <string>
using namespace std;

/*****************************************************************************
Usage:
-----

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "");

    const char* ctest = "äóaôÚbcñÑ";

    std::string stest = std::string(ctest);
    std::string::iterator it = stest.begin();
    UTF8Iterator iterator(it);

    char32_t code = *iterator;
    for(int iIndex = 0; iIndex < 9; iIndex++) {
        printf("%c -> %d\n", code, code);
        iterator++;
        code = *iterator;
    }

    std::cout << endl;
    system("PAUSE");

    return 0;
}

******************************************************************************/

class UTF8Iterator : public std::iterator<
    std::bidirectional_iterator_tag, char32_t,
    std::string::difference_type,
    const char32_t*,
    const char32_t&>
{
private:

    const unsigned char m_k128  = 128;  // 1000000
    const unsigned char m_k64   = 64;   // 0100000
    const unsigned char m_k32   = 32;   // 0010000
    const unsigned char m_k16   = 16;   // 0001000
    const unsigned char m_k8    = 8;    // 0000100

    mutable char32_t    m_currentCodePoint;
    mutable bool        m_dirty;

    std::string::const_iterator m_stringIterator;

public:

    UTF8Iterator(std::string::const_iterator i_iterator) :
        m_stringIterator(i_iterator),
        m_currentCodePoint(0),
        m_dirty(true){}

    UTF8Iterator(std::string i_string) :
        m_stringIterator(i_string.begin()),
        m_currentCodePoint(0),
        m_dirty(true){}

    UTF8Iterator(const UTF8Iterator& i_source) :
        m_stringIterator(i_source.m_stringIterator),
        m_currentCodePoint(i_source.m_currentCodePoint),
        m_dirty(i_source.m_dirty){}

    ~UTF8Iterator(){}

public:

    UTF8Iterator operator++(int)
    {
        UTF8Iterator temp = *this;
        ++(*this);
        return temp;
    }

    UTF8Iterator& operator++()
    {
        char firstByte = *m_stringIterator;
        std::string::difference_type offset = 1;
        if(firstByte & m_k128) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.
        {
            if(firstByte & m_k32) // This means that the first byte has a value greater than 224, and so it must be at least a three-octet code point.
            {
                if(firstByte & m_k16) // This means that the first byte has a value greater than 240, and so it must be a four-octet code point.
                    offset = 4;
                else
                    offset = 3;
            }
            else
                offset = 2;
        }
        m_stringIterator += offset;

        m_dirty = true;
        return *this;
    }

    UTF8Iterator& operator--()
    {
        --m_stringIterator;
        if(*m_stringIterator & m_k128) // This means that the previous byte is not an ASCII character.
        {
            --m_stringIterator;
            if((*m_stringIterator & m_k64) == 0)
            {
                --m_stringIterator;
                if((*m_stringIterator & m_k64) == 0)
                    --m_stringIterator;
            }
        }

        m_dirty = true;
        return *this;
    }

    UTF8Iterator operator--(int)
    {
        UTF8Iterator temp = *this;
        --(*this);
        return temp;
    }

    char32_t operator*() const
    {
        CalculateCurrentCodePoint();
        return m_currentCodePoint;
    }

    UTF8Iterator& operator=(const UTF8Iterator& i_rhs)
    {
        m_stringIterator = i_rhs.m_stringIterator;
        m_currentCodePoint = i_rhs.m_currentCodePoint;
        m_currentCodePoint = i_rhs.m_dirty;
        return *this;
    }

    bool operator==(const UTF8Iterator& i_rhs) const
    {
        return m_stringIterator ==  i_rhs.m_stringIterator;
    }

    bool operator!=(const UTF8Iterator& i_rhs) const
    {
        return m_stringIterator !=  i_rhs.m_stringIterator;
    }

    bool operator==(std::string::iterator i_rhs) const
    {
        return m_stringIterator ==  i_rhs;
    }

    bool operator==(std::string::const_iterator i_rhs) const
    {
        return m_stringIterator ==  i_rhs;
    }

    bool operator!=(std::string::iterator i_rhs) const
    {
        return m_stringIterator !=  i_rhs;
    }

    bool operator!=(std::string::const_iterator i_rhs) const
    {
        return m_stringIterator !=  i_rhs;
    }

private:

    void CalculateCurrentCodePoint(/*Converts UTF8 -> Unicode*/) const
    {
        if(m_dirty)
        {
            char32_t codePoint = 0;
            char firstByte = *m_stringIterator;
            if(firstByte & m_k128) // This means the first byte has a value greater than 127, and so is beyond the ASCII range.
            {
                if(firstByte & m_k32) // This means that the first byte has a value greater than 191, and so it must be at least a three-octet code point.
                {
                    if(firstByte & m_k16) // This means that the first byte has a value greater than 224, and so it must be a four-octet code point.
                    {
                        codePoint = (firstByte & 0x07) << 18;
                        char secondByte = *(m_stringIterator + 1);
                        codePoint +=  (secondByte & 0x3f) << 12;
                        char thirdByte = *(m_stringIterator + 2);
                        codePoint +=  (thirdByte & 0x3f) << 6;;
                        char fourthByte = *(m_stringIterator + 3);
                        codePoint += (fourthByte & 0x3f);
                    }
                    else
                    {
                        codePoint = (firstByte & 0x0f) << 12;
                        char secondByte = *(m_stringIterator + 1);
                        codePoint += (secondByte & 0x3f) << 6;
                        char thirdByte = *(m_stringIterator + 2);
                        codePoint +=  (thirdByte & 0x3f);
                    }
                }
                else
                {
                    codePoint = (firstByte & 0x1f) << 6;
                    char secondByte = *(m_stringIterator + 1);
                    codePoint +=  (secondByte & 0x3f);
                }
            }
            else
                codePoint = firstByte;

            m_currentCodePoint = codePoint;
            m_dirty = false;
        }
    }
};

#endif // UTF8Iterator_H
