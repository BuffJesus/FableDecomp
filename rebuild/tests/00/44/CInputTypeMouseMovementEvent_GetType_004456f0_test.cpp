#include <stdio.h>

int __fastcall GetType(void *this_ptr)
{
    (void)this_ptr;
    return 5;
}

int main(void)
{
    int r = GetType((void*)0);
    if (r != 5) { printf("FAIL got %d\n", r); return 1; }
    printf("OK_0x004456f0 GetType=%d\n", r);
    return 0;
}