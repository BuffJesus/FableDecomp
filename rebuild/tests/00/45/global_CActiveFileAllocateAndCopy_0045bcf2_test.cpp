#include <stdio.h>
static int AllocatedBytes(int count) { return count == 0 ? 0 : count * 4; }
int main() { if (AllocatedBytes(0) != 0 || AllocatedBytes(7) != 28) return 1; puts("CACTIVEFILE_ALLOC_COPY_PASS"); return 0; }