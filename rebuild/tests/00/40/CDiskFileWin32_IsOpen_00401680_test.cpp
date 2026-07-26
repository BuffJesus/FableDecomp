#include <cstdio>

struct CDiskFileWin32 {
    char pad[0x14];
    bool m_isOpen;
};

bool __fastcall CDiskFileWin32_IsOpen(const CDiskFileWin32* self)
{
    return self->m_isOpen;
}

int main()
{
    CDiskFileWin32 a;
    a.m_isOpen = true;
    CDiskFileWin32 b;
    b.m_isOpen = false;
    if (CDiskFileWin32_IsOpen(&a) == true && CDiskFileWin32_IsOpen(&b) == false) {
        std::printf("CDiskFileWin32_00401680_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}