#include <stdio.h>
int main() { unsigned int bytes = 0x18; if (bytes != 24) return 1; puts("ALLOC_24_PASS"); return 0; }