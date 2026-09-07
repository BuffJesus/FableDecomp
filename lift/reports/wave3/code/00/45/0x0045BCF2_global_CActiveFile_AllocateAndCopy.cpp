#include <cstdlib>
#include <cstddef>

void __fastcall CActiveFile_CopyRange(int a2, int a3, void* destination, const void* source);

void* __stdcall CActiveFile_AllocateAndCopy(int elementCount, int a2, int a3)
{
    void* buffer;

    if (elementCount == 0) {
        buffer = nullptr;
    } else {
        buffer = std::malloc(elementCount << 2);
    }

    CActiveFile_CopyRange(
        a2,
        a3,
        buffer,
        static_cast<const void*>(reinterpret_cast<const unsigned char*>(&elementCount) + 3));

    return buffer;
}