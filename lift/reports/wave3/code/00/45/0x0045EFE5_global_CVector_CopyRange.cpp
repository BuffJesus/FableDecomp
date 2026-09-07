#include <cstring>
#include <cstdint>

void* __fastcall CVector_CopyRange(void* first, void* last, void* dest)
{
    if (last != first)
    {
        const int byteCount =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(last)) -
            static_cast<int>(reinterpret_cast<std::uintptr_t>(first));

        dest = std::memmove(dest, first, byteCount);
        dest = reinterpret_cast<void*>(
            static_cast<int>(reinterpret_cast<std::uintptr_t>(dest)) + byteCount);
    }

    return dest;
}