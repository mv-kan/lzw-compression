#if !defined(KLZWCOMPRESS_H)
#define KLZWCOMPRESS_H

#include <iostream>
#include <vector>
namespace klzw
{
    namespace details
    {
        // convert codes to bytes
        // returns size_t value that indicates how many free bits there are in the last byte of bytes
        // if CodesToBytes sees ExtendCode value then it will add one bit to all next values
        size_t CodesToBytes(const std::vector<code_t>& codes, size_t codeSize, std::vector<byte>&bytes);
    } // namespace details
    
    void Compress(std::string inputfile, std::string outputfile);
} // namespace klzw

#endif