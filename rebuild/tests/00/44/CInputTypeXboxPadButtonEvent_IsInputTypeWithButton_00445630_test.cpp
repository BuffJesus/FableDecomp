#include <stdio.h>
extern "C" bool IsInputTypeWithButton(void *self, int button) { return *(int *)((char *)self + 4) == button; }
int main() {
    char obj[8]; *(int *)(obj + 4) = 12;
    if (!IsInputTypeWithButton(obj, 12)) { printf("FAIL equal\n"); return 1; }
    if (IsInputTypeWithButton(obj, 13)) { printf("FAIL unequal\n"); return 1; }
    printf("PASS_00445630\n");
    return 0;
}