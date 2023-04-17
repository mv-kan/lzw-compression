#include <stdexcept>
#include "decompress.h"

namespace klzw
{
    void Decompress(std::string inputfile, std::string outputfile)
    {
        std::cout << inputfile << std::endl;
        std::cout << outputfile << std::endl;
        throw std::runtime_error("not implemented");
    }
} // namespace klzw