#include <cstring>

void* __fastcall Copy_MemoryRange(void* source_begin, void* source_end, void* destination)
{
    if (source_end != source_begin) {
        destination = std::memmove(
            destination,
            source_begin,
            static_cast<int>(reinterpret_cast<int>(source_end) - reinterpret_cast<int>(source_begin)));
        destination = reinterpret_cast<void*>(
            reinterpret_cast<int>(destination) +
            (reinterpret_cast<int>(source_end) - reinterpret_cast<int>(source_begin)));
    }

    return destination;
}