#include <cstdio>

static void* g_received_this = 0;

extern "C" __declspec(naked) void RealTarget_0x63d960(void)
{
    __asm
    {
        mov g_received_this, ecx
        ret
    }
}

__declspec(naked) void DrawFindLastUnblockedGroundPositionAndReportCollision(void)
{
    __asm
    {
        add ecx, -4
        jmp RealTarget_0x63d960
    }
}

int main(void)
{
    unsigned char storage[64];
    void* base = (void*)&storage[16];

    g_received_this = 0;
    __asm
    {
        mov ecx, base
        call DrawFindLastUnblockedGroundPositionAndReportCollision
    }

    void* expected = (void*)((unsigned char*)base - 4);
    if (g_received_this == expected)
    {
        std::printf("OK_0x004fb290\n");
        return 0;
    }
    std::printf("FAIL_0x004fb290 got=%p expected=%p\n", g_received_this, expected);
    return 1;
}