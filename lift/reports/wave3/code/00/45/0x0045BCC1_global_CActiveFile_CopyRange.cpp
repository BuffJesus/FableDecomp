#include <cstdint>
#include <cstring>

void* __fastcall CActiveFile_CopyRange(void* param_1, void* param_2, void* param_3)
{
    if (param_2 != param_1) {
        const int byteCount =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(param_2)) -
            static_cast<int>(reinterpret_cast<std::uintptr_t>(param_1));

        param_3 = std::memmove(param_3, param_1, byteCount);
        param_3 = static_cast<void*>(static_cast<unsigned char*>(param_3) + byteCount);
    }

    return param_3;
}