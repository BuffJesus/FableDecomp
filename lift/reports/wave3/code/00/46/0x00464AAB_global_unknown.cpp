#include <cstddef>
#include <cstring>

void* __fastcall Vector_CopyRangeForward(void* first, void* last, void* dest)
{
    if (last != first) {
        const std::ptrdiff_t byte_count =
            static_cast<unsigned char*>(last) - static_cast<unsigned char*>(first);

        dest = std::memmove(dest, first, static_cast<std::size_t>(byte_count));
        dest = static_cast<unsigned char*>(dest) + byte_count;
    }

    return dest;
}