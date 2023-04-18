#include "utils.h"

namespace klzw
{
    namespace details
    {
        code_t StopCode(size_t bits) {
            return MaxValue(bits);
        }
        code_t ExtendCode(size_t bits) {
            return MaxValue(bits) - 1;
        }

    } // namespace details
}