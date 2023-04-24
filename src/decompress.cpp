#include <stdexcept>
#include <fstream>
#include "decompress.h"
#include "decomptable.h"
namespace klzw
{
    namespace details
    {
        void BytesToCodes(const std::vector<byte> &bytes, size_t *offset, size_t *byteshift, ssize_t *codeSizeVar, std::vector<code_t> &codes, size_t codeSize)
        {
            if (codes.size() == 0)
                codes.push_back(0);

            size_t &_offset{*offset};
            size_t currentCodeIndex{};
            ssize_t &_codeSize{*codeSizeVar};
            size_t &_byteshift{*byteshift};
            size_t maxByteValue{MaxValue(BITS_IN_BYTE)};
            size_t bytesize{bytes.size()};
            for (size_t i = 0; i < bytesize; i++)
            {
                size_t byte = static_cast<size_t>(bytes[i]);
                if (_codeSize < static_cast<ssize_t>(BITS_IN_BYTE))
                    byte &= maxByteValue >> (BITS_IN_BYTE - _codeSize - _offset);
                codes[currentCodeIndex] += (byte << _byteshift) >> _offset;
                _byteshift += BITS_IN_BYTE;
                _codeSize -= BITS_IN_BYTE;

                // reset and parse next code
                if (_codeSize < 0)
                {
                    // codeSize - is negative here
                    _offset = (BITS_IN_BYTE + _codeSize + _offset) % BITS_IN_BYTE;
                    _byteshift = 0;
                    // if we have extend code parsed then we extend code by 1
                    if (codes[currentCodeIndex] == ExtendCode(codeSize))
                    {
                        codeSize += 1;
                    }
                    currentCodeIndex++;
                    codes.push_back(0);
                    // // (bytesize - i) how many bytes are left to process
                    // // if we have bits to process we will add new elem
                    // // if left bits are less than codeSize it means we finish the conversion
                    // if (static_cast<ssize_t>((bytesize - i) * BITS_IN_BYTE) - static_cast<ssize_t>(codeSize) > 0)
                    // {
                        
                        
                    // }
                    _codeSize = codeSize;
                    // if we have offset then we have some bits to read in this iteration
                    // otherwise we have read all bits in "i" byte
                    if (_offset > 0)
                        i--;
                }
            }
        }
    } // namespace details

    void Decompress(std::string inputfile, std::string outputfile)
    {
        std::ifstream _inputfile(inputfile, std::ios::in | std::ios::binary);
        std::ofstream _outputfile(outputfile, std::ios::out | std::ios::binary);
        if (_inputfile.is_open() && _outputfile.is_open())
        {
            // table (dictionary) of code -> str
            details::decomptable table{};

            // buffer, just reading from file stuff
            const size_t BUF_SIZE{100};
            char buf[BUF_SIZE];

            // this is bufbytes vector for BytesToCode func
            std::vector<byte> bufbytes{};
            bufbytes.reserve(BUF_SIZE);

            std::vector<code_t> codes{};
            codes.reserve(BUF_SIZE);

            std::vector<code_t> outputcodes{};
            outputcodes.reserve(BUF_SIZE);

            // these bytes we write to output file
            std::vector<byte> outputbytes{};
            outputbytes.reserve(BUF_SIZE * 2);

            size_t byteoffset{};
            size_t byteshift{};
            ssize_t codeSize{static_cast<ssize_t>(table.CodeSize())};
            size_t oldCodeSize{table.CodeSize()};
            bool isFirstByte{true};
            code_t oldcode{};

            // to save last code if invalid
            code_t tmp{};
            while (_inputfile.read(buf, BUF_SIZE) || _inputfile.gcount())
            {
                size_t bytesread = static_cast<size_t>(_inputfile.gcount());
                // if we have left bytes in bufbytes we shouldn't overwrite them
                bufbytes.insert(bufbytes.begin() + bufbytes.size(), std::begin(buf), buf + bytesread);

                details::BytesToCodes(bufbytes, &byteoffset, &byteshift, &codeSize, codes, oldCodeSize);
                // this is in case if we got code extension, BytesToCode handles it but it need old code size to do conversion properly
                oldCodeSize = table.CodeSize();
                if (codeSize > 0)
                {
                    // if codeSize is more than zero then last code is not valid
                    tmp = codes[codes.size() - 1];
                    codes.pop_back();
                }
                for (size_t i = 0; i < codes.size(); i++)
                {
                    std::cout << "i = " << i << std::endl;
                    if (isFirstByte)
                    {
                        outputcodes.push_back(codes[i]);
                        oldcode = codes[i];
                        isFirstByte = false;
                    }
                    else
                    {
                        if (codes[i] == table.ExtendCode())
                        {
                            table.ExtendCodeSize();
                            // do not add extend code to outputcodes
                            continue;
                        }
                        std::vector<byte> str = table.Get(codes[i]);
                        // codes[i] does not exist
                        // TODO MAN this is so unoptimized
                        if (str.size() == 0)
                        {
                            auto strold = table.Get(oldcode);
                            strold.push_back(strold[0]);
                            // table strold's code should be codes[i] because it is first encounter with unknown code
                            table.Set(strold);
                            outputcodes.push_back(codes[i]);
                            oldcode = codes[i];
                        }
                        else
                        {
                            outputcodes.push_back(codes[i]);
                            auto strold = table.Get(oldcode);
                            strold.push_back(str[0]);
                            table.Set(strold);
                            oldcode = codes[i];
                        }
                    }
                }

                // convert outputcodes to bytes
                for (size_t i = 0; i < outputcodes.size(); i++)
                {
                    auto str = table.Get(outputcodes[i]);
                    outputbytes.insert(outputbytes.begin() + outputbytes.size(), str.begin(), str.end());
                }

                _outputfile.write(reinterpret_cast<const char *>(&outputbytes[0]), outputbytes.size());

                // flush everything
                outputbytes.resize(0);
                outputcodes.resize(0);
                codes.resize(0);
                if (codeSize > 0)
                {
                    // last code that is invalid
                    codes.push_back(tmp);
                }
                bufbytes.resize(0);
            }

            _inputfile.close();
            _outputfile.close();
        }
        else
        {
            if (!_inputfile.is_open())
            {
                throw std::runtime_error("unable to open input file \"" + inputfile + "\"");
            }
            else
            {
                throw std::runtime_error("unable to open output file \"" + outputfile + "\"");
            }
        }
    }
} // namespace klzw