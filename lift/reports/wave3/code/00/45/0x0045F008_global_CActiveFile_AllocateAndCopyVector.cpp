#include <cstdlib>
#include <cstddef>

extern void __fastcall CVector_CopyRange(const void* a1, const void* a2, void* a3, const void* a4);

void* __stdcall CActiveFile_AllocateAndCopyVector(int count, const void* a2, const void* a3)
{
    void* result;

    if (count == 0) {
        result = nullptr;
    } else {
        result = std::malloc(count << 2);
    }

    CVector_CopyRange(
        a2,
        a3,
        result,
        reinterpret_cast<const std::byte*>(&count) + 3
    );

    return result;
}