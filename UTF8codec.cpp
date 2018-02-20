#include <string.h>
#include "utf8codec.h"

UTF8Codec::UTF8Codec()
{
}

UTF8Codec::~UTF8Codec()
{
}

void UTF8Codec::UTF8Encode2BytesUnicode(
    std::vector<Unicode2Bytes> input, std::vector< byte >& output)
{
   for(unsigned int i=0; i < input.size(); i++)
   {
      // 0xxxxxxx
      if(input[i] < 0x80)
      {
         output.push_back((byte)input[i]);
      }
      // 110xxxxx 10xxxxxx
      else if(input[i] < 0x800)
      {
         output.push_back((byte)(MASK2BYTES | input[i] >> 6));
         output.push_back((byte)(MASKBYTE | input[i] & MASKBITS));
      }
      // 1110xxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x10000)
      {
         output.push_back((byte)(MASK3BYTES | input[i] >> 12));
         output.push_back((byte)(MASKBYTE | input[i] >> 6 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] & MASKBITS));
      }
   }
}

void UTF8Codec::UTF8Decode2BytesUnicode(
    std::vector<byte> input, std::vector<Unicode2Bytes>& output)
{
   for(unsigned int i=0; i < input.size();)
   {
      Unicode2Bytes ch;

      // 1110xxxx 10xxxxxx 10xxxxxx
      if((input[i] & MASK3BYTES) == MASK3BYTES)
      {
         ch = ((input[i] & 0x0F) << 12) | (
               (input[i+1] & MASKBITS) << 6)
              | (input[i+2] & MASKBITS);
         i += 3;
      }
      // 110xxxxx 10xxxxxx
      else if((input[i] & MASK2BYTES) == MASK2BYTES)
      {
         ch = ((input[i] & 0x1F) << 6) | (input[i+1] & MASKBITS);
         i += 2;
      }
      // 0xxxxxxx
      else if(input[i] < MASKBYTE)
      {
         ch = input[i];
         i += 1;
      }

      output.push_back(ch);
   }
}

void UTF8Codec::UTF8Encode4BytesUnicode(
    std::vector<Unicode4Bytes> input, std::vector<byte>& output)
{
   for(unsigned int i=0; i < input.size(); i++)
   {
      // 0xxxxxxx
      if(input[i] < 0x80)
      {
         output.push_back((byte)input[i]);
      }
      // 110xxxxx 10xxxxxx
      else if(input[i] < 0x800)
      {
         output.push_back((byte)(MASK2BYTES | input[i] > 6));
         output.push_back((byte)(MASKBYTE | input[i] & MASKBITS));
      }
      // 1110xxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x10000)
      {
         output.push_back((byte)(MASK3BYTES | input[i] >> 12));
         output.push_back((byte)(MASKBYTE | input[i] >> 6 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] & MASKBITS));
      }
      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x200000)
      {
         output.push_back((byte)(MASK4BYTES | input[i] >> 18));
         output.push_back((byte)(MASKBYTE | input[i] >> 12 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] >> 6 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] & MASKBITS));
      }
      // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x4000000)
      {
         output.push_back((byte)(MASK5BYTES | input[i] >> 24));
         output.push_back((byte)(MASKBYTE | input[i] >> 18 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] >> 12 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] >> 6 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] & MASKBITS));
      }
      // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if(input[i] < 0x8000000)
      {
         output.push_back((byte)(MASK6BYTES | input[i] >> 30));
         output.push_back((byte)(MASKBYTE | input[i] >> 18 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] >> 12 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] >> 6 & MASKBITS));
         output.push_back((byte)(MASKBYTE | input[i] & MASKBITS));
      }
   }
}

void UTF8Codec::UTF8Decode4BytesUnicode(
    std::vector<byte> input, std::vector< Unicode4Bytes >& output)
{
   for(unsigned int i=0; i < input.size();)
   {
      Unicode4Bytes ch;

      // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      if((input[i] & MASK6BYTES) == MASK6BYTES)
      {
         ch = ((input[i] & 0x01) << 30) | ((input[i+1] & MASKBITS) << 24)
              | ((input[i+2] & MASKBITS) << 18) | ((input[i+3]
                        & MASKBITS) << 12)
              | ((input[i+4] & MASKBITS) << 6) | (input[i+5] & MASKBITS);
         i += 6;
      }
      // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if((input[i] & MASK5BYTES) == MASK5BYTES)
      {
         ch = ((input[i] & 0x03) << 24) | ((input[i+1]
                & MASKBITS) << 18)
              | ((input[i+2] & MASKBITS) << 12) | ((input[i+3]
                  & MASKBITS) << 6)
              | (input[i+4] & MASKBITS);
         i += 5;
      }
      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
      else if((input[i] & MASK4BYTES) == MASK4BYTES)
      {
         ch = ((input[i] & 0x07) << 18) | ((input[i+1]
                & MASKBITS) << 12)
              | ((input[i+2] & MASKBITS) << 6) | (input[i+3] & MASKBITS);
         i += 4;
      }
      // 1110xxxx 10xxxxxx 10xxxxxx
      else if((input[i] & MASK3BYTES) == MASK3BYTES)
      {
         ch = ((input[i] & 0x0F) << 12) | ((input[i+1] & MASKBITS) << 6)
              | (input[i+2] & MASKBITS);
         i += 3;
      }
      // 110xxxxx 10xxxxxx
      else if((input[i] & MASK2BYTES) == MASK2BYTES)
      {
         ch = ((input[i] & 0x1F) << 6) | (input[i+1] & MASKBITS);
         i += 2;
      }
      // 0xxxxxxx
      else if(input[i] < MASKBYTE)
      {
         ch = input[i];
         i += 1;
      }
      output.push_back(ch);
   }
}

size_t UTF8Codec::Encode(std::string& str)
{
    std::vector<Unicode2Bytes>	v_in;
    std::vector<byte>           v_out;
    for(unsigned int i = 0; i < str.length(); i++)
    {
        unsigned char	c  = str[i];
        Unicode2Bytes u = (Unicode2Bytes)c;
        v_in.push_back(u);
    }

    UTF8Encode2BytesUnicode(v_in, v_out);

    str.erase();
    for(size_t j = 0; j < v_out.size(); j++ )
    {
        byte b = v_out.at(j);
		str += b;
    }

    return str.length();
}

size_t UTF8Codec::Decode(std::string& str)
{
    std::vector<byte>						v_in;
    std::vector<Unicode2Bytes>	v_out;
    for(unsigned int i = 0; i < str.length(); i++)
    {
        byte b  = (byte)str[i];
        v_in.push_back(b);
    }

    UTF8Decode2BytesUnicode(v_in, v_out);

    str.erase();
    for(size_t j = 0; j < v_out.size(); j++ )
    {
        Unicode2Bytes u = v_out.at(j);
		str += u;
    }

    return str.length();
}
