#include <stdio.h>
static int AllocationBytes(int count) { return count == 0 ? 0 : count * 4; }
int main() { if (AllocationBytes(0) != 0 || AllocationBytes(3) != 12) return 1; puts("CACTIVEFILE_ALLOC_ARRAY_PASS"); return 0; }