#include <stdio.h>
extern "C" unsigned long GetConstantCount(void *self) {
    char *p = (char *)self;
    return (unsigned long)((*(long *)(p + 0x7bc) - *(long *)(p + 0x7b8)) >> 3);
}
int main() {
    char obj[0x800]; int i;
    for (i = 0; i < (int)sizeof(obj); ++i) obj[i] = 0;
    *(long *)(obj + 0x7b8) = 0x1000;
    *(long *)(obj + 0x7bc) = 0x1030;
    if (GetConstantCount(obj) != 6) { printf("FAIL count\n"); return 1; }
    printf("PASS_00445940\n");
    return 0;
}