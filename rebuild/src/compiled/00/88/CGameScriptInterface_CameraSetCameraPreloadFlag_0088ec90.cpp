struct Provider { void* vt; };
struct Owner { char pad[0x34]; Provider* prov; };
struct Self { char pad[4]; Owner* owner; };

void __fastcall CGameScriptInterface_CameraSetCameraPreloadFlag(Self* self, void* edx, char flag)
{
    void* local;
    Provider* prov = self->owner->prov;
    void** vt = *(void***)prov;
    typedef void (__fastcall *fn_t)(Provider*, void*, void**);
    fn_t fn = (fn_t)vt[9];
    fn(prov, vt, &local);
    *((char*)local + 0x145) = flag;
}