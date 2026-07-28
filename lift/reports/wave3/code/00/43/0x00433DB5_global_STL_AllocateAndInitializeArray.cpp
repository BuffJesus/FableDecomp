#include <cstddef>
#include <cstdlib>

extern "C" void __fastcall _Uninit_copy(
    const void* first,
    const void* last,
    void* destination,
    void* construction_cookie);

extern "C" void* __stdcall STL_AllocateAndInitializeArray(
    int count,
    const void* first,
    const void* last)
{
    void* allocation;

    if (count == 0) {
        allocation = nullptr;
    } else {
        allocation = std::malloc(
            static_cast<std::size_t>(static_cast<unsigned int>(count) << 2));
    }

    _Uninit_copy(first, last, allocation, reinterpret_cast<unsigned char*>(&count) + 3);
    return allocation;
}