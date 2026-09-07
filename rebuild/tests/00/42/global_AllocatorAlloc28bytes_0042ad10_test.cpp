#include <stdio.h>
int main() { unsigned int bytes = 0x1c; if (bytes != 28) return 1; puts("ALLOC_28_PASS"); return 0; }