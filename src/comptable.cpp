#include "comptable.h"
#include <vector>
namespace klzw
{
    namespace details
    {

        comptable::comptable()
        {
            // fill the table with 1 byte values (from 0 to 255)
            code_t max = details::MaxValue(details::BITS_IN_BYTE);
            for (code_t i = 0; i <= max; i++)
            {
                std::vector<byte> str{static_cast<byte>(i)};

                _table[str] = i;
            }
            _nextAvailableCode = max + 1;
        }

        code_t comptable::StopCode() const
        {
            return details::MaxValue(_codeSize);
        }

        code_t comptable::ExtentCodeSizeCode() const
        {
            return details::MaxValue(_codeSize) - 1;
        }

        void comptable::Set(std::vector<byte> str)
        {
            _table[str] = _nextAvailableCode;
            _nextAvailableCode += 1;
        }

        code_t comptable::Get(std::vector<byte> str) const
        {
            auto got = _table.find(str);
            if (got == _table.end())
                return StopCode();
            else
                return got->second;
        }
    } // namespace details
} // namespace klzw
