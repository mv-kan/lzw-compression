#if !defined(COMPTABLE_H)
#define COMPTABLE_H

#include <iostream>
#include <unordered_map>
#include "utils.h"

namespace klzw
{
    // compresssion table
    class comptable
    {
    private:
        // TODO: maybe use string_view with really long string stream
        std::unordered_map<std::string, code_t> _table;
        
        size_t _codeSize{details::BITS_IN_BYTE + 1};

        code_t _nextAvailableCode{};
    public:
        // inits all 8 byte values automatically
        comptable();
        ~comptable() = default;
        // get current code size
        inline size_t CodeSize() const { return _codeSize; }

        // get stop code for this code size
        code_t StopCode() const;

        // get extent code size code 
        code_t ExtentCodeSizeCode() const;
        
        // set str in table with next available code
        void Set(std::string str);

        // get str code in str
        // returns StopCode if value str is not found
        code_t Get(std::string str) const;
    };
} // namespace klzw


#endif