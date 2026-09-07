#include "engine/CDiskFileWin32.h"
#include <cstdio>


bool __fastcall CDiskFileWin32_IsOpen(const CDiskFileWin32* self)
{
    return self->Opened;
}

int main()
{
    CDiskFileWin32 a;
    a.Opened = true;
    CDiskFileWin32 b;
    b.Opened = false;
    if (CDiskFileWin32_IsOpen(&a) == true && CDiskFileWin32_IsOpen(&b) == false) {
        std::printf("CDiskFileWin32_00401680_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}