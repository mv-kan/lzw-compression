#include "decomptable.h"

namespace klzw
{
    namespace details
    {
        decomptable::decomptable()
        {
            // fill the table with 1 byte values (from 0 to 255)
            code_t max = details::MaxValue(details::BITS_IN_BYTE);
            for (code_t i = 0; i <= max; i++)
            {
                std::vector<byte> str{static_cast<byte>(i)};

                _table[i] = std::move(str);
            }
            _nextAvailableCode = max + 1;
        }
        void decomptable::Set(std::vector<byte> str)
        {
            _table[_nextAvailableCode] = str;
            _nextAvailableCode += 1;
        }
        std::vector<byte> decomptable::Get(code_t code) const
        {
            auto got = _table.find(code);
            if (got == _table.end())
                return {};
            else
                return got->second;
        }

    } // namespace details
} // namespace klzw
