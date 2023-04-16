#include <stdexcept>
#include <fstream>
#include <iostream>
#include "utils.h"
#include "compress.h"
#include "comptable.h"
namespace klzw
{
    void Compress(std::string inputfile, std::string outputfile)
    {
        std::ifstream _inputfile(inputfile, std::ios::in | std::ios::binary);
        if (_inputfile.is_open()) {
            details::comptable table{};
            const size_t BUF_SIZE{100};
            char buf[BUF_SIZE + 1];
            // read file
            while (_inputfile.read(buf, BUF_SIZE) || _inputfile.gcount())
            {
                buf[_inputfile.gcount()] = '\0';
                
                // compression happens here
                std::cout << buf << std::endl;
            }
            _inputfile.close();
        } else {
            throw std::runtime_error("unable to open file \"" + inputfile + "\"");
        }
    }
} // namespace klzw