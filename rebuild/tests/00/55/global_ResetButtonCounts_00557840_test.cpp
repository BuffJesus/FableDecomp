#include <stdio.h>

int g_buttonCountA;
int g_buttonCountB;

void ResetButtonCounts(void)
{
    g_buttonCountA = 0;
    g_buttonCountB = 0;
}

int main(void)
{
    g_buttonCountA = 111;
    g_buttonCountB = 222;
    ResetButtonCounts();
    if (g_buttonCountA == 0 && g_buttonCountB == 0) {
        printf("OK_0x00557840\n");
        return 0;
    }
    printf("FAIL a=%d b=%d\n", g_buttonCountA, g_buttonCountB);
    return 1;
}