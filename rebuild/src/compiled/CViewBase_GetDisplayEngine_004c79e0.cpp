struct CDisplayEngine;
struct CViewBase;
struct SomeGlobal { char pad[0x28]; CDisplayEngine* engine; };
extern SomeGlobal* g_someGlobal;

CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(const CViewBase* self)
{
    (void)self;
    return g_someGlobal->engine;
}