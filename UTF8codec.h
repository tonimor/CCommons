#ifndef UTF8CODEC_H
#define UTF8CODEC_H

#include <vector>
#include <string>
using namespace std;


/*****************************************************************************
Usage to encode/decode Strings:
-------------------------------
((LPCTSTR)CUTF8Codec::Encode(string_to_encode);
((LPCTSTR)CUTF8Codec::Decode(string_to_decode);

******************************************************************************/

typedef unsigned char   byte;           // 8  bits: 0 <= x <= 255
typedef unsigned short  Unicode2Bytes;  // 16 bits: 0 <= x <= 65535
typedef unsigned int    Unicode4Bytes;  // 32 bits: 0 <= x <= 4294967295

class UTF8Codec
{
public:
    enum
    {
        MASKBITS    = 0x3F,
        MASKBYTE    = 0x80,
        MASK2BYTES  = 0xC0,
        MASK3BYTES  = 0xE0,
        MASK4BYTES  = 0xF0,
        MASK5BYTES  = 0xF8,
        MASK6BYTES  = 0xFC,
    };

    UTF8Codec();
    virtual ~UTF8Codec();

    static void UTF8Encode2BytesUnicode(
        std::vector<Unicode2Bytes> input, std::vector<byte>& output);

    static void UTF8Decode2BytesUnicode(
        std::vector<byte> input, std::vector< Unicode2Bytes >& output);

    static void UTF8Encode4BytesUnicode(
        std::vector<Unicode4Bytes> input, std::vector<byte>& output);

    static void UTF8Decode4BytesUnicode(
        std::vector<byte> input, std::vector<Unicode4Bytes>& output);

    static size_t Encode(std::string&);
    static size_t Decode(std::string&);
};

#endif // UTF8CODEC_H

