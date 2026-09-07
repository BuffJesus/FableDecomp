#include <stdio.h>
static int Compute(int value) { return value / 255 + 1 + value; }
int main() { if (Compute(0) != 1 || Compute(254) != 255 || Compute(255) != 257 || Compute(-255) != -255) return 1; puts("MATH_BYTE_CHUNK_PASS"); return 0; }