#include <cstring>
#include <cstdint>

void* __fastcall Vector_CopyRange(void* param_1, void* param_2, void* param_3)
{
    if (param_2 != param_1) {
        const int byteCount =
            static_cast<int>(reinterpret_cast<std::intptr_t>(param_2)) -
            static_cast<int>(reinterpret_cast<std::intptr_t>(param_1));

        param_3 = std::memmove(param_3, param_1, static_cast<std::size_t>(byteCount));
        param_3 = reinterpret_cast<void*>(
            static_cast<int>(reinterpret_cast<std::intptr_t>(param_3)) + byteCount);
    }

    return param_3;
}