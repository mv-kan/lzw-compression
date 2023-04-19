#if !defined(KLZWDECOMPRESS_H)
#define KLZWDECOMPRESS_H

#include <iostream>
#include <vector>
#include "utils.h"
namespace klzw
{
    namespace details
    {
        // writes to codes bytes 
        // returns offset of last byte, how many free bits is there
        // @bytes - bytes to convert to codes
        // @offset - offset of bytes, how many to skip from first byte
        // @codes - vector where to store all codes
        // @codeSize - code size in bits  
        size_t BytesToCodes(const std::vector<byte> &bytes, size_t offset, std::vector<code_t> &codes, size_t codeSize);
    } // namespace details
    
    void Decompress(std::string inputfile, std::string outputfile);
} // namespace klzw

#endif