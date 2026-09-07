#include <stdio.h>
int main() { unsigned int bytes = 0x38; if (bytes != 56) return 1; puts("ALLOC_56_PASS"); return 0; }