#include <cstdio>

// Function-under-test: same lowering as retail (mov eax,7 ; ret).
int __fastcall GetType(void* ecx)
{
    (void)ecx;
    return 7;
}

int main()
{
    // Invoke through a thiscall-style shim: this=ecx.
    char obj[16];
    void* self = obj;
    int result;
    __asm {
        mov ecx, self
        call GetType
        mov result, eax
    }
    if (result != 7) {
        printf("FAIL result=%d\n", result);
        return 1;
    }
    printf("OK_0x00445770 GetType=%d\n", result);
    return 0;
}