#include <stdio.h>
extern "C" bool IsInputTypeWithButton(void *, int) { return false; }
int main() {
    if (IsInputTypeWithButton((void*)0x1234, 7) != false) { printf("FAIL expected false\n"); return 1; }
    printf("PASS_00447a00\n");
    return 0;
}