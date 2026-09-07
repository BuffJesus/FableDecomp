#include <cstdint>
#include <cstdlib>

void CActiveFile_AssignVector8(
    int count,
    std::uint32_t* first,
    std::uint32_t* last)
{
    std::uint32_t* destination;
    void* allocated;
    int delta;

    if (count == 0) {
        allocated = nullptr;
    } else {
        allocated = std::malloc(count << 3);
    }

    if (first != last) {
        delta = (int)allocated - (int)first;
        do {
            destination = (std::uint32_t*)(delta + (int)first);
            if (destination != nullptr) {
                *destination = *first;
                *(std::uint32_t*)(delta + 4 + (int)first) = first[1];
            }
            first = first + 2;
        } while (first != last);
    }
}