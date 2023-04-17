#include <stdexcept>
#include <fstream>
#include <iostream>
#include "utils.h"
#include "compress.h"
#include "comptable.h"
#include <vector>
namespace klzw
{
    void Compress(std::string inputfile, std::string outputfile)
    {
        std::ifstream _inputfile(inputfile, std::ios::in | std::ios::binary);
        if (_inputfile.is_open()) {
            // table (dictionary) of str -> code 
            details::comptable table{};

            // buffer, just reading from file stuff
            const size_t BUF_SIZE{100};
            char buf[BUF_SIZE + 1];
            
            // our string, in which we are going to store chars
            std::vector<byte> str{};
            str.reserve(BUF_SIZE);
            
            // where we store codes
            std::vector<code_t> codestream{}; 
            codestream.reserve(BUF_SIZE);     

            // read file
            while (_inputfile.read(buf, BUF_SIZE) || _inputfile.gcount())
            {
                buf[_inputfile.gcount()] = '\0';

                // compression happens here
                // convert bytes to codes, codes are stored in codestream
                for (size_t i = 0; i < _inputfile.gcount(); i++)
                {
                    // buf[i] - next char in charstream
                    code_t stopCode = table.StopCode();
                    str.push_back(buf[i]);
                    code_t got = table.Get(str);
                    // if not found
                    if (got == stopCode) {
                        code_t extendCode = table.ExtendCode();
                        if (table.NextCode() == extendCode) {
                            table.ExtendCodeSize();
                            // add extendCode to codestream
                            codestream.push_back(extendCode);
                        }
                        // add str to table with new code
                        table.Set(str);

                        // revert str to state without buf[i] char
                        str.pop_back();

                        code_t code = table.Get(str); // guaranteed that code is found and not stopCode
                        codestream.push_back(code);
                        
                        // set str to buf[i]
                        str.resize(0);
                        str.push_back(buf[i]);
                    } 
                }

                // convert codestream to bytes and write them to outputfile
                // also last offset byte we gonna  
                
                // write code to outputfile
                // add new strings to table 
                // see that video about lzw compression 
            }
            _inputfile.close();
        } else {
            throw std::runtime_error("unable to open file \"" + inputfile + "\"");
        }
    }
} // namespace klzw