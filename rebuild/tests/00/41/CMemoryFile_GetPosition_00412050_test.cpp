#include <cstdio>

struct CMemoryFile {
    int pad0;
    int pad4;
    long position;
};

long __fastcall CMemoryFile_GetPosition(const CMemoryFile* self) {
    return self->position;
}

int main() {
    CMemoryFile f;
    f.pad0 = 111;
    f.pad4 = 222;
    f.position = 1234567;
    if (CMemoryFile_GetPosition(&f) != 1234567) {
        std::printf("FAIL: wrong position\n");
        return 1;
    }
    f.position = -9;
    if (CMemoryFile_GetPosition(&f) != -9) {
        std::printf("FAIL: negative position\n");
        return 1;
    }
    std::printf("CMemoryFile_00412050_TEST PASS\n");
    return 0;
}