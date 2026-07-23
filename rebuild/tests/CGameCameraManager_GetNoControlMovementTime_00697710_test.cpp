#include <stdio.h>

struct CGameCameraManager {
    char _pad[0xC0];
    int  m_noControlStartTime; // +0xC0
};

extern int __cdecl GetEngineTimeMs(void);
extern float g_defaultNoControlMovementTime;
extern int   g_noControlTimeDivisor;

float __fastcall CGameCameraManager_GetNoControlMovementTime(CGameCameraManager *self)
{
    if (self->m_noControlStartTime == 0)
        return g_defaultNoControlMovementTime;
    return (float)(GetEngineTimeMs() - self->m_noControlStartTime) / g_noControlTimeDivisor;
}

// ---- trivial extern stub defs so the test links ----
float g_defaultNoControlMovementTime = -1.0f;
int   g_noControlTimeDivisor = 1000;
static int g_fakeNow = 5500;
int __cdecl GetEngineTimeMs(void) { return g_fakeNow; }

int main(void)
{
    CGameCameraManager cm;
    for (int i = 0; i < (int)sizeof(cm); ++i) ((char*)&cm)[i] = 0;

    // Case 1: start time == 0 -> returns the default global (fld path).
    cm.m_noControlStartTime = 0;
    float r0 = CGameCameraManager_GetNoControlMovementTime(&cm);

    // Case 2: start time set -> (now - start)/divisor. now=5500 -> (5500-2500)/1000 = 3.0
    cm.m_noControlStartTime = 2500;
    float r1 = CGameCameraManager_GetNoControlMovementTime(&cm);

    int ok = (r0 == -1.0f) && (r1 == 3.0f);

    printf("r0=%f r1=%f\n", r0, r1);
    if (ok)
        printf("CGAMECAMERAMANAGER_00697710_TEST PASS\n");
    else
        printf("CGAMECAMERAMANAGER_00697710_TEST FAIL\n");
    return ok ? 0 : 1;
}