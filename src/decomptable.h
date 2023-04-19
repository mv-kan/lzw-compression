#if !defined(DECOMPTABLE_H)
#define DECOMPTABLE_H

#include <unordered_map>
#include <vector>
#include "utils.h"

namespace klzw
{
    namespace details
    {
        class decomptable
        {
        private:
            std::unordered_map<code_t, std::vector<byte>> _table;

            size_t _codeSize{details::BITS_IN_BYTE + 1};

            code_t _nextAvailableCode{};

        public:
            // init all variables and _table
            decomptable();
            ~decomptable() = default;

            // get current code size
            inline size_t CodeSize() const { return _codeSize; }

            // get next available code
            inline code_t NextCode() const { return _nextAvailableCode; };

            // get stop code for this code size
            inline code_t StopCode() const { return details::StopCode(_codeSize); }

            // get extent code size code
            // this code shows to compressor that we need to extend code size by 1
            inline code_t ExtendCode() const { return details::ExtendCode(_codeSize); }

            inline void ExtendCodeSize()
            {
                _codeSize += 1;
            }

            // set str in table with next available code
            void Set(std::vector<byte> str);

            // get str by code
            // returns vector.size() = 0 if value code is not found
            std::vector<byte> Get(code_t code) const;
        };
    } // namespace details

} // namespace klzw

#endif