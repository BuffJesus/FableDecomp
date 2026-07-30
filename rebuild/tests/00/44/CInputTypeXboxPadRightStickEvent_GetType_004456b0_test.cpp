#include <cstdio>

// Function under test: CInputTypeXboxPadRightStickEvent::GetType
// Retail 0x004456b0: mov eax, 4 ; ret  (__fastcall, this in ecx, unused)
int __fastcall GetType(void* ecx)
{
    (void)ecx;
    return 4;
}

int main()
{
    void* fakeThis = (void*)0xDEADBEEF;
    int r;
    // Invoke via __asm shim: put this in ecx (__fastcall convention).
    __asm {
        mov ecx, fakeThis
        call GetType
        mov r, eax
    }
    if (r != 4) {
        printf("FAIL got %d\n", r);
        return 1;
    }
    // Also verify it ignores 'this' (call with a different pointer).
    void* other = (void*)0;
    __asm {
        mov ecx, other
        call GetType
        mov r, eax
    }
    if (r != 4) {
        printf("FAIL2 got %d\n", r);
        return 1;
    }
    printf("OK_0x004456b0\n");
    return 0;
}