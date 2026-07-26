#include <cstdio>

struct CDiskFileWin32 {
    char pad[0xc];
    long length;
};

long __fastcall CDiskFileWin32_GetLength(const CDiskFileWin32* self) {
    return self->length;
}

int main() {
    CDiskFileWin32 obj;
    obj.length = 12345;
    if (CDiskFileWin32_GetLength(&obj) != 12345) {
        std::printf("FAIL: wrong length\n");
        return 1;
    }
    obj.length = -7;
    if (CDiskFileWin32_GetLength(&obj) != -7) {
        std::printf("FAIL: wrong length neg\n");
        return 1;
    }
    std::printf("CDiskFileWin32_00401670_TEST PASS\n");
    return 0;
}