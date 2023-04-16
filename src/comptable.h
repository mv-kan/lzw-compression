#if !defined(COMPTABLE_H)
#define COMPTABLE_H

#include <iostream>
#include <unordered_map>
#include "utils.h"
#include <vector>

namespace klzw
{
    namespace details
    {
        struct VectorHasher
        {
            int operator()(const std::vector<byte> &V) const
            {
                int hash = V.size();
                for (auto &i : V)
                {
                    hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                }
                return hash;
            }
        };

        // compresssion table
        class comptable
        {
        private:
            // I use vector<byte> for string because default std::string won't allow /null (0) char in the middle or beginning
            std::unordered_map<std::vector<byte>, code_t, details::VectorHasher> _table;

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
            void Set(std::vector<byte> str);

            // get str code in str
            // returns StopCode if value str is not found
            code_t Get(std::vector<byte> str) const;
        };
    } // namespace details
    
    
} // namespace klzw


#endif