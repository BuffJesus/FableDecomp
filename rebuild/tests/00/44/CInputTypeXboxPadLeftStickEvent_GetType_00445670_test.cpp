#include <cstdio>

// ---- function under test (mirror of source_cpp) ----
int __fastcall GetType(void *ecx)
{
    (void)ecx;
    return 3;
}

int main()
{
    // Invoke via __asm shim: this=ecx (__fastcall/__fastcall), ret 0 frame.
    void *obj = (void *)0xDEADBEEF; // arbitrary this; must be ignored
    int r;
    __asm {
        mov ecx, obj
        call GetType
        mov r, eax
    }
    if (r != 3) {
        printf("FAIL got %d\n", r);
        return 1;
    }
    printf("OK_0x00445670 GetType=%d\n", r);
    return 0;
}