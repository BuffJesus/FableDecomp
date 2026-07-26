#include <cstddef>
#include <cstring>

void* __fastcall Std_Move_Backward(void* first, void* last, void* dest)
{
    if (last != first)
    {
        const auto count = static_cast<std::size_t>(
            reinterpret_cast<std::byte*>(last) - reinterpret_cast<std::byte*>(first));

        dest = std::memmove(dest, first, count);
        dest = reinterpret_cast<std::byte*>(dest) + count;
    }

    return dest;
}