#include <stdio.h>
static int g_calls = 0;
static void *g_last = 0;
extern "C" void ConstructWideFloatPair(void *dst, void *src) { ++g_calls; g_last = src; }
extern "C" void _Cons_val(void *dst, void *src) { if (dst) ConstructWideFloatPair(dst, src); }
int main() {
    int a = 1, b = 2;
    _Cons_val(0, &b);
    if (g_calls != 0) { printf("FAIL null constructed\n"); return 1; }
    _Cons_val(&a, &b);
    if (g_calls != 1 || g_last != &b) { printf("FAIL construct path\n"); return 1; }
    printf("PASS_0044b110\n");
    return 0;
}