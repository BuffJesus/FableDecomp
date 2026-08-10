// CEngineResourceManager::ProcessLoading @ 00b3b8c0
// fld [g]; sub esp,8; fstp [esp]; push 0; add ecx,-0xc; call 0xb3b7f0; ret
extern double g_procLoadTime;

struct SubObj {
    void Do(int a, double d);   // extern -> emits a call, cleans 12 bytes (__fastcall)
};

struct CEngineResourceManager {
    char pad[0x10];
};

void __fastcall ProcessLoading(CEngineResourceManager* self)
{
    SubObj* s = reinterpret_cast<SubObj*>(reinterpret_cast<char*>(self) - 0xc);
    s->Do(0, g_procLoadTime);
}