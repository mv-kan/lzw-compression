#include <stdexcept>
#include "decompress.h"
#include "decomptable.h"

namespace klzw
{
    namespace details
    {
        size_t BytesToCodes(const std::vector<byte> &bytes, size_t offset, std::vector<code_t> &codes, size_t codeSize)
        {
            
        }
    } // namespace details

    void Decompress(std::string inputfile, std::string outputfile)
    {
        std::cout << inputfile << std::endl;
        std::cout << outputfile << std::endl;
        throw std::runtime_error("not implemented");
    }
} // namespace klzw