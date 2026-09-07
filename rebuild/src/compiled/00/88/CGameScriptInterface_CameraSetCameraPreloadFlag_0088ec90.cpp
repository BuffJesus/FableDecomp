#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Provider { void* vt; };
struct CWorld { char pad[0x34]; Provider* prov; };

void __fastcall CGameScriptInterface_CameraSetCameraPreloadFlag(CGameScriptInterface* self, void* edx, char flag)
{
    void* local;
    Provider* prov = self->World->prov;
    void** vt = *(void***)prov;
    typedef void (__fastcall *fn_t)(Provider*, void*, void**);
    fn_t fn = (fn_t)vt[9];
    fn(prov, vt, &local);
    *((char*)local + 0x145) = flag;
}