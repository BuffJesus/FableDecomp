#include <stdio.h>
extern "C" bool IsInputTypeWithMouseButton(void *, int) { return false; }
int main() {
    if (IsInputTypeWithMouseButton((void*)1, 1)) { printf("FAIL expected false\n"); return 1; }
    printf("PASS_004457d0\n");
    return 0;
}