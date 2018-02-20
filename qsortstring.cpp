#include "qsortstring.h"

QSortString::QSortString()
{
    m_arrayToSort = NULL;
}

QSortString::QSortString(StringArray* i_arrayToSort)
{
    m_arrayToSort = i_arrayToSort;
}

void QSortString::QSort(StringArray& i_arrayToSort)
{
    pThis = this;
    QSortObject::QSort(i_arrayToSort);
}

void QSortString::QSort()
{
    pThis = this;
    QSortObject::QSort(*m_arrayToSort);
}

int QSortString::QSortCompareFunction(const void *i_arg1, const void *i_arg2)
{
    std::string	string1 = m_stringBuffer[*(unsigned long *)i_arg1];
    std::string	string2 = m_stringBuffer[*(unsigned long *)i_arg2];
    if(string1 > string2)
        return 1;
    else if(string1 < string2)
        return -1;
    return 0;
}
