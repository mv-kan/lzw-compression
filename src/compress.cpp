#include "lzw.h"
#include <stdexcept>
#include <compress.h>
#include <fstream>
#include <iostream>
#include "utils.h"

namespace klzw
{
    void Compress(std::string inputfile, std::string outputfile)
    {
        std::ifstream _inputfile(inputfile, std::ios::in | std::ios::binary);
        if (_inputfile.is_open()) {
            char buf[101];
            // read file
            while (_inputfile.read(buf, 100)) {
                buf[_inputfile.gcount()] = '\0';
                std::cout << buf;
            }
            // read all what left from file
            if (_inputfile.eof() && _inputfile.gcount() > 0)
            {                                       // Handle any remaining characters in the file
                buf[_inputfile.gcount()] = '\0';    // Null-terminate the buffer after the last character read
                std::cout << buf;
            }
            _inputfile.close();
        } else {
            throw std::runtime_error("unable to open file \"" + inputfile + "\"");
        }
    }
} // namespace klzw