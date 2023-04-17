#include <stdexcept>
#include <fstream>
#include <iostream>
#include "utils.h"
#include "compress.h"
#include "comptable.h"
#include <vector>
namespace klzw
{
    namespace details
    {
        size_t CodesToBytes(const std::vector<code_t> &codes, size_t codeSize, std::vector<byte> &bytes, size_t offset) {
            size_t _offset{offset};
            size_t currentByteIndex{};
            if (bytes.size() == 0)
                bytes.push_back(0);

            
            for (size_t i = 0; i < codes.size(); ++i)
            {
                size_t code = codes[i];
                ssize_t _codeSize = codeSize;
                if (code == MaxValue(codeSize) - 1) {
                    codeSize += 1;
                }
                while (code || _codeSize > 0)
                {
                    bytes[currentByteIndex] += (code << _offset) & MaxValue(BITS_IN_BYTE);
                    code >>= BITS_IN_BYTE - _offset;
                    _codeSize -= BITS_IN_BYTE - _offset;
                    // if code size is bigger than 0 then we just add another byte
                    if (_codeSize > 0)
                    {
                        bytes.push_back(0);
                        currentByteIndex++;

                        // we dont have offset here so we reset it
                        _offset = 0;
                    }
                    else
                    {                                      // otherwise we change offset value,
                        _offset = BITS_IN_BYTE + _codeSize; // this is subtraction because codeSize is negative here
                    }
                }
            }
            return _offset;
        }
    } // namespace details
    
    void Compress(std::string inputfile, std::string outputfile)
    {
        std::ifstream _inputfile(inputfile, std::ios::in | std::ios::binary);
        std::ofstream _outputfile(outputfile, std::ios::binary);
        if (_inputfile.is_open() && _outputfile.is_open()) {
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

            std::vector<byte> bytes{};
            bytes.reserve(BUF_SIZE * 2);

            size_t byteoffset{};
            // read file
            while (_inputfile.read(buf, BUF_SIZE) || _inputfile.gcount())
            {
                // buf[_inputfile.gcount()] = '\0';
                size_t oldCodeSize = table.CodeSize();
                size_t bytesread = static_cast<size_t>(_inputfile.gcount());
                // compression happens here
                // convert bytes to codes, codes are stored in codestream
                for (size_t i = 0; i < bytesread; i++)
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
                // we need oldcodesize because if table.CodeSize changed then in codestream we have extendcode
                // that will show to CodesToBytes that we need to extend our code 
                byteoffset = details::CodesToBytes(codestream, oldCodeSize, bytes, byteoffset);
                
                // if byteoffset is bigger than 0 it means that we have free bits in last byte
                // we will not write last byte if we have space in the last byte
                // instead we arrange bytes in that way that in the next iteration we will have
                // the byte at the first(0) index  
                size_t writebytes = byteoffset > 0 ? bytes.size() - 1 : bytes.size();
                _outputfile.write(reinterpret_cast<const char *>(&bytes[0]), writebytes);

                // if we have free bits in last byte, we save it in the first of bytes for next iteration
                if (byteoffset > 0) {
                    bytes[0] = bytes[bytes.size() - 1];
                    bytes.resize(1);
                } else {
                    bytes.resize(0);
                }

            }
            _inputfile.close();
            _outputfile.close();
        } else {
            if (_inputfile.is_open())
            {
                throw std::runtime_error("unable to open input file \"" + inputfile + "\"");
            } else {
                throw std::runtime_error("unable to open output file \"" + outputfile + "\"");
            }
        }
    }
} // namespace klzw