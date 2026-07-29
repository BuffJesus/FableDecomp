#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/99/CMemoryDataInputStream_OpenMemorySource_00994570.cpp"

struct TestMemoryDataInputStream_00994570
{
    void* vtable;
    unsigned long position;
    unsigned long totalSize;
    void* source;
    unsigned long sourcePosition;
    unsigned long sourceBytes;
    void* originalSource;
};

int main()
{
    typedef void (__fastcall *OpenFunction)(
        void*,
        void*,
        void*,
        unsigned long);
    OpenFunction volatile open =
        &CMemoryDataInputStream_OpenMemorySource_00994570;

    int failures = 0;
    unsigned char source[32];
    TestMemoryDataInputStream_00994570 stream;
    std::memset(&stream, 0xA5, sizeof(stream));
    void* const originalVtable = stream.vtable;

    open(&stream, 0, source, sizeof(source));
    if (stream.vtable != originalVtable ||
        stream.position != 0 ||
        stream.totalSize != sizeof(source) ||
        stream.source != source ||
        stream.sourcePosition != 0 ||
        stream.sourceBytes != sizeof(source) ||
        stream.originalSource != source)
    {
        ++failures;
    }

    std::memset(&stream, 0x5A, sizeof(stream));
    void* const secondVtable = stream.vtable;
    open(&stream, 0, 0, 0);
    if (stream.vtable != secondVtable ||
        stream.position != 0 ||
        stream.totalSize != 0 ||
        stream.source != 0 ||
        stream.sourcePosition != 0 ||
        stream.sourceBytes != 0 ||
        stream.originalSource != 0)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "MEMORY_INPUT_OPEN_00994570_TEST FAIL count=%d position=%lu size=%lu\n",
            failures,
            stream.position,
            stream.totalSize);
        return 1;
    }

    std::printf("MEMORY_INPUT_OPEN_00994570_TEST PASS\n");
    return 0;
}
