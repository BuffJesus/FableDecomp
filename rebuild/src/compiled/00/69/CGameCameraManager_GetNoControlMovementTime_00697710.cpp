// CGameCameraManager::GetNoControlMovementTime @ 0x00697710 (VC7.1 / MSVC 2003)
// This-only member accessor modeled as __fastcall (object ptr arrives in ecx,
// byte-identical to __fastcall for a this-only method). Compile: cl /c /O2 /Oy /W3

struct CGameCameraManager {
    char _pad[0xC0];
    int  m_noControlStartTime; // +0xC0
};

// Engine callee: returns a monotonically increasing millisecond tick (int in eax).
extern int __cdecl GetEngineTimeMs(void);

// Relocation-masked globals read directly by the accessor.
extern float g_defaultNoControlMovementTime; // ds:0x122dedc, read via fld
extern int   g_noControlTimeDivisor;         // ds:0x1375550, read via fidiv (integer divisor)

float __fastcall CGameCameraManager_GetNoControlMovementTime(CGameCameraManager *self)
{
    if (self->m_noControlStartTime == 0)
        return g_defaultNoControlMovementTime;

    return (float)(GetEngineTimeMs() - self->m_noControlStartTime) / g_noControlTimeDivisor;
}