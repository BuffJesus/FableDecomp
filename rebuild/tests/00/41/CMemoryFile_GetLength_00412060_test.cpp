#include <cstdio>

struct CMemoryFile {
    int _pad0;
    int _pad4;
    int _pad8;
    long m_length;
};

long __fastcall CMemoryFile_GetLength(const CMemoryFile* self)
{
    return self->m_length;
}

int main()
{
    CMemoryFile f;
    f._pad0 = 111;
    f._pad4 = 222;
    f._pad8 = 333;
    f.m_length = 4242;
    long r = CMemoryFile_GetLength(&f);
    if (r != 4242) {
        std::printf("CMemoryFile_00412060_TEST FAIL got %ld\n", r);
        return 1;
    }
    std::printf("CMemoryFile_00412060_TEST PASS\n");
    return 0;
}