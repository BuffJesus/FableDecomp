#include <stdio.h>
unsigned long g_cookie = 0x12345678;
int g_failed = 0;
void check(unsigned long c)
{
    if (c == g_cookie) return;
    g_failed = 1;
}
int main()
{
    g_failed = 0;
    check(0x12345678);
    if (g_failed) { printf("FAIL match\n"); return 1; }
    check(0xDEADBEEF);
    if (!g_failed) { printf("FAIL mismatch\n"); return 1; }
    printf("OK_00bfe9f9\n");
    return 0;
}