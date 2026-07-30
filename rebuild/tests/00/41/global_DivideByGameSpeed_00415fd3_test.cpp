#include <stdio.h>

int g_gameSpeed = 4;

float __stdcall DivideByGameSpeed(float x)
{
    return x / g_gameSpeed;
}

int main(void)
{
    g_gameSpeed = 4;
    float r = DivideByGameSpeed(10.0f);
    if (r == 2.5f) {
        g_gameSpeed = 2;
        float r2 = DivideByGameSpeed(9.0f);
        if (r2 == 4.5f) {
            printf("OK_0x00415fd3 ");
            return 0;
        }
    }
    printf("FAIL ");
    return 1;
}