#include <stdio.h>

// Function under test: returns constant type id 6.
int __fastcall GetType(void *ecx)
{
    (void)ecx;
    return 6;
}

int main(void)
{
    int dummy = 0;
    int r;
    void *self = &dummy;
    // Invoke with this in ecx via a __fastcall shim.
    __asm {
        mov ecx, self
        call GetType
        mov r, eax
    }
    if (r != 6) {
        printf("FAIL got %d\n", r);
        return 1;
    }
    printf("OK_0x00445730\n");
    return 0;
}