#include "comptable.h"

namespace klzw
{
    comptable::comptable() {
        code_t max = details::MaxValue(details::BITS_IN_BYTE);
        for (code_t i = 0; i <= max; i++)
        {
            std::string str{static_cast<char>(i)};
            
            _table[str] = i;
        }
        _nextAvailableCode = max + 1;
    }

    code_t comptable::StopCode() const {
        return details::MaxValue(_codeSize);
    }

    code_t comptable::ExtentCodeSizeCode() const {
        return details::MaxValue(_codeSize) - 1;
    }

    void comptable::Set(std::string str) {
        _table[str] = _nextAvailableCode;
        _nextAvailableCode += 1;
    }

    code_t comptable::Get(std::string str) const {
        auto got = _table.find(str);
        if (got == _table.end())
            return StopCode();
        else 
            return got->second;
    }

} // namespace klzw
