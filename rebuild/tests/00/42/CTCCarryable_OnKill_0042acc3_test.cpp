#include <stdio.h>
int main()
{
    const char *p = "PASS_0042acc3";
    if (!p || !p[0]) return 1;
    printf("%s\n", p);
    return 0;
}