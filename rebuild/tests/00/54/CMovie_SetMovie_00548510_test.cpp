#include <cstdio>

// Behaviour capture: the real target records the this pointer it was tail-jumped
// to, so we can assert the thunk added exactly 0x174 to ecx before jumping.
static void* g_received_this = 0;

extern "C" __declspec(naked) void RealTarget_0x99b7d0(void)
{
    __asm
    {
        mov g_received_this, ecx
        ret
    }
}

__declspec(naked) void SetMovie(void)
{
    __asm
    {
        add ecx, 0x174
        jmp RealTarget_0x99b7d0
    }
}

int main(void)
{
    unsigned char storage[0x200];
    void* base = (void*)&storage[0];

    g_received_this = 0;
    __asm
    {
        mov ecx, base
        call SetMovie
    }

    void* expected = (void*)((unsigned char*)base + 0x174);
    if (g_received_this == expected)
    {
        std::printf("OK_0x00548510\n");
        return 0;
    }
    std::printf("FAIL_0x00548510 got=%p expected=%p\n", g_received_this, expected);
    return 1;
}