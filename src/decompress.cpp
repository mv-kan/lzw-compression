#include <stdexcept>
#include <fstream>
#include "decompress.h"
#include "decomptable.h"
namespace klzw
{
    namespace details
    {
        void BytesToCodes(const std::vector<byte> &bytes, size_t *offset, size_t *byteshift, ssize_t *codeSizeVar, std::vector<code_t> &codes, size_t *codeSizeTable)
        {
            if (codes.size() == 0)
                codes.push_back(0);

            size_t &_offset{*offset};
            size_t currentCodeIndex{};
            ssize_t &_codeSize{*codeSizeVar};
            size_t &_byteshift{*byteshift};
            size_t &_codeSizeTable{*codeSizeTable};
            size_t maxByteValue{MaxValue(BITS_IN_BYTE)};
            size_t bytesize{bytes.size()};
            for (size_t i = 0; i < bytesize; i++)
            {
                size_t byte = static_cast<size_t>(bytes[i]);
                if (_codeSize < static_cast<ssize_t>(BITS_IN_BYTE))
                {
                    ssize_t shift = BITS_IN_BYTE - _codeSize - _offset;
                    if (shift > 0)
                    {
                        byte &= maxByteValue >> shift;
                    }
                }
                codes[currentCodeIndex] += (byte << _byteshift) >> _offset;
                _byteshift += BITS_IN_BYTE;
                _codeSize -= BITS_IN_BYTE;

                // reset and parse next code
                // codeSize + static_cast<ssize_t>(byteoffset) <= 0 meaning of this exp
                // if true then it means we finished parsing currentCodeIndex code and we can move on
                if (_codeSize + static_cast<ssize_t>(_offset) <= 0) 
                {
                    // codeSize - is negative here
                    _offset = (BITS_IN_BYTE + _codeSize + _offset);

                    // if we have offset then we have some bits to read in this iteration
                    // otherwise we have read all bits in "i" byte
                    if (_offset != 0 && _offset < BITS_IN_BYTE) 
                    {
                        i--;
                    }
                    _offset %= BITS_IN_BYTE;
                    _byteshift = 0;

                    // if we have extend code parsed then we extend code by 1
                    if (codes[currentCodeIndex] == ExtendCode(_codeSizeTable))
                    {
                        _codeSizeTable += 1;
                    }
                    currentCodeIndex++;
                    codes.push_back(0);

                    _codeSize = _codeSizeTable;
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
            const size_t BUF_SIZE{1000}; // TODO in decompress we have problem with buffer
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
            code_t oldcode{};

            bool isFirstByte{true};

            // to save last code if invalid
            code_t tmp{};
            while (_inputfile.read(buf, BUF_SIZE) || _inputfile.gcount())
            {
                size_t bytesread = static_cast<size_t>(_inputfile.gcount());
                // if we have left bytes in bufbytes we shouldn't overwrite them
                bufbytes.insert(bufbytes.begin() + bufbytes.size(), std::begin(buf), buf + bytesread);

                details::BytesToCodes(bufbytes, &byteoffset, &byteshift, &codeSize, codes, &oldCodeSize);
                // this is in case if we got code extension, BytesToCode handles it but it need old code size to do conversion properly
                if (codeSize + static_cast<ssize_t>(byteoffset) > 0)
                {
                    // if codeSize is more than zero then last code is not valid
                    tmp = codes[codes.size() - 1];
                    codes.pop_back();
                }
                // we have bits of info in the last byte, so we save it for next iteration
                if (codeSize + static_cast<ssize_t>(byteoffset) <= 0 && byteoffset != 0 && byteoffset < details::BITS_IN_BYTE)
                {
                    bufbytes[0] = bufbytes[bufbytes.size() - 1];
                    bufbytes.resize(1);
                } else {
                    // flush completely
                    bufbytes.resize(0);
                }

                for (size_t i = 0; i < codes.size(); i++)
                {
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
                // bufbytes.resize(0); already done in the beginning of iteration
                if (codeSize + static_cast<ssize_t>(byteoffset) > 0)
                {
                    // last code that is invalid
                    codes.push_back(tmp);
                }
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