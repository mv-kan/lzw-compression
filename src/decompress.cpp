#include <stdexcept>
#include <fstream>
#include "decompress.h"
#include "decomptable.h"

namespace klzw
{
    namespace details
    {
        size_t BytesToCodes(const std::vector<byte> &bytes, size_t offset, std::vector<code_t> &codes, size_t codeSize)
        {
            if (codes.size() == 0)
                codes.push_back(0);

            size_t _offset{offset};
            size_t currentCodeIndex{};
            ssize_t _codeSize{static_cast<ssize_t>(codeSize)};
            size_t byteshift{};
            size_t maxByteValue{MaxValue(BITS_IN_BYTE)};
            size_t bytesize{bytes.size()};
            for (size_t i = 0; i < bytesize; i++)
            {
                size_t byte = static_cast<size_t>(bytes[i]);
                if (_codeSize < static_cast<ssize_t>(BITS_IN_BYTE))
                    byte &= maxByteValue >> (BITS_IN_BYTE - _codeSize - _offset);
                codes[currentCodeIndex] += (byte << byteshift) >> _offset;
                byteshift += BITS_IN_BYTE;
                _codeSize -= BITS_IN_BYTE;

                // reset and parse next code
                if (_codeSize < 0)
                {
                    // (bytesize - i + 1) how many bytes are left to process
                    // if we have bits to process we will add new elem
                    // if left bits are less than codeSize it means we finish the conversion
                    if (static_cast<ssize_t>((bytesize - i) * BITS_IN_BYTE) - static_cast<ssize_t>(codeSize) > 0)
                    {
                        // if we have extend code parsed then we extend code by 1
                        if (codes[currentCodeIndex] == ExtendCode(codeSize))
                        {
                            codeSize += 1;
                        }
                        currentCodeIndex++;
                        codes.push_back(0);
                    }
                    // codeSize - is negative here
                    _offset = (BITS_IN_BYTE + _codeSize + _offset) % BITS_IN_BYTE;
                    _codeSize = codeSize;
                    byteshift = 0;
                    // if we have offset then we have some bits to read in this iteration
                    // otherwise we have read all bits in "i" byte
                    if (_offset > 0)
                        i--;
                }
            }
            return _offset;
        }
    } // namespace details

    void Decompress(std::string inputfile, std::string outputfile)
    {
        std::ifstream _inputfile(inputfile, std::ios::in | std::ios::binary);
        std::ofstream _outputfile(outputfile, std::ios::binary);
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

            // these bytes we write to output file
            std::vector<byte> bytes{};
            bytes.reserve(BUF_SIZE * 2);

            size_t byteoffset{};

            while (_inputfile.read(buf, BUF_SIZE) || _inputfile.gcount())
            {
                size_t bytesread = static_cast<size_t>(_inputfile.gcount());
                bufbytes.insert(bufbytes.begin(), std::begin(buf), std::end(buf));

                byteoffset = details::BytesToCodes(bufbytes, byteoffset, codes, table.CodeSize());
                
                for (size_t i = 0; i < codes.size(); i++)
                {
                    table.Get(codes[i]);
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