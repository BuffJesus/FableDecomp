#include <cstdint>
#include <cstring>

void* __fastcall Vector_CopyRangeForward(void* begin, void* end, void* out)
{
    if (end != begin) {
        const int count =
            static_cast<int>(reinterpret_cast<std::uintptr_t>(end)) -
            static_cast<int>(reinterpret_cast<std::uintptr_t>(begin));

        out = std::memmove(out, begin, static_cast<std::size_t>(count));
        out = reinterpret_cast<void*>(
            static_cast<int>(reinterpret_cast<std::uintptr_t>(out)) + count);
    }

    return out;
}