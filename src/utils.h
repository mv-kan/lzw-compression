#if !defined(KLZWUTILS_H)
#define KLZWUTILS_H

#include <iostream>
#include <limits>
namespace klzw
{
    using byte = unsigned char;
    using code_t = size_t;
    namespace details
    {
        const size_t BITS_IN_BYTE{8};

        // get max value for n bits
        inline code_t MaxValue(size_t bits) {
            code_t max = std::numeric_limits<code_t>::max();

            return max >> ((sizeof(code_t) * 8) - bits);
        }

    } // namespace details
}
#endif