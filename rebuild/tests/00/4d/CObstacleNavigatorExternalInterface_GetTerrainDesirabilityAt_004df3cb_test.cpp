#include <cstdio>

// Definition of the function-under-test (same lowering as retail).
float g_terrainDesirability; // ds:0x122ded8

struct CObstacleNavigatorExternalInterface {
    float GetTerrainDesirabilityAt(void* pos);
};

float CObstacleNavigatorExternalInterface::GetTerrainDesirabilityAt(void* pos)
{
    (void)pos;
    return g_terrainDesirability;
}

int main()
{
    CObstacleNavigatorExternalInterface obj;
    void* self = &obj;
    void* arg  = (void*)0x1234;

    // Case 1
    g_terrainDesirability = 0.75f;
    float r1;
    // __fastcall: this in ecx, one 4-byte stack arg (ret 4)
    __asm {
        mov  ecx, self
        push arg
        call CObstacleNavigatorExternalInterface::GetTerrainDesirabilityAt
        fstp r1
    }

    // Case 2 - different global value
    g_terrainDesirability = -2.5f;
    float r2;
    __asm {
        mov  ecx, self
        push arg
        call CObstacleNavigatorExternalInterface::GetTerrainDesirabilityAt
        fstp r2
    }

    if (r1 == 0.75f && r2 == -2.5f) {
        printf("OK_0x004df3cb r1=%f r2=%f\n", r1, r2);
        return 0;
    }
    printf("FAIL r1=%f r2=%f\n", r1, r2);
    return 1;
}