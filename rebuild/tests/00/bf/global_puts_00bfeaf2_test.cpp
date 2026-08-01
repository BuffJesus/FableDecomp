#include <stdio.h>
int main() {
    int r = puts("hello_from_puts_thunk");
    printf("RESULT r=%d PASS_MARK\n", r);
    return 0;
}