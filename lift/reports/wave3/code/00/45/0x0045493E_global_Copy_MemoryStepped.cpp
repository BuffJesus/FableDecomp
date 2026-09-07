#include <cstring>
#include <cstdint>

void* __fastcall Copy_MemoryStepped(void* src_begin, void* src_end, void* dst)
{
    if (src_end != src_begin) {
        const int byte_count =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(src_end) -
                             reinterpret_cast<std::uintptr_t>(src_begin));

        dst = std::memmove(dst, src_begin, byte_count);
        dst = reinterpret_cast<void*>(
            reinterpret_cast<std::uintptr_t>(dst) + byte_count);
    }

    return dst;
}