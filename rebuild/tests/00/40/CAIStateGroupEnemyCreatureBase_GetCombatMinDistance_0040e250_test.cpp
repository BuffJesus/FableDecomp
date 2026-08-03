#include <stdio.h>

// Definition of the global the accessor reads.
float g_CombatMinDistance = 0.0f;

// Copy of the function-under-test.
float GetCombatMinDistance()
{
    return g_CombatMinDistance;
}

int main()
{
    g_CombatMinDistance = 12.5f;
    float r = GetCombatMinDistance();
    if (r != 12.5f) {
        printf("FAIL got %f\n", r);
        return 1;
    }
    g_CombatMinDistance = -3.25f;
    if (GetCombatMinDistance() != -3.25f) {
        printf("FAIL got %f\n", GetCombatMinDistance());
        return 1;
    }
    printf("OK_0x0040e250\n");
    return 0;
}