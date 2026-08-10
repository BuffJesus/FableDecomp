#include <stdio.h>
// Behavioural model of the SetStateAfterYesNo head:
// - copies a security cookie into the frame
// - loads a global flag byte and compares it against zero (ebx=0)
// The observable head-decision is: flag != 0 -> ZF clear, flag == 0 -> ZF set.
static unsigned char g_flag;

static int decide(unsigned char flag)
{
    g_flag = flag;
    unsigned char al = g_flag;
    unsigned char bl = 0;
    // cmp al, bl : returns whether they are equal (ZF)
    return (al == bl) ? 1 : 0;
}

int main(void)
{
    int ok = 1;
    if (decide(0) != 1) ok = 0;   // flag==0 -> equal
    if (decide(1) != 0) ok = 0;   // flag==1 -> not equal
    if (decide(0xFF) != 0) ok = 0;
    if (ok) printf("OK_005eaf30\n");
    else    printf("FAIL_005eaf30\n");
    return ok ? 0 : 1;
}