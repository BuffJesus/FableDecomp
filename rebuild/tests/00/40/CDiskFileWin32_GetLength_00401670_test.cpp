#include "engine/CDiskFileWin32.h"
#include <cstdio>


long __fastcall CDiskFileWin32_GetLength(const CDiskFileWin32* self) {
    return self->Length;
}

int main() {
    CDiskFileWin32 obj;
    obj.Length = 12345;
    if (CDiskFileWin32_GetLength(&obj) != 12345) {
        std::printf("FAIL: wrong length\n");
        return 1;
    }
    obj.Length = -7;
    if (CDiskFileWin32_GetLength(&obj) != -7) {
        std::printf("FAIL: wrong length neg\n");
        return 1;
    }
    std::printf("CDiskFileWin32_00401670_TEST PASS\n");
    return 0;
}