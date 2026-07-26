class CDisplayEngine;
struct CViewBase {};
struct GlobalHolder { char pad[0x30]; CDisplayEngine* engine; };
extern GlobalHolder* g_displayHolder;
CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(CViewBase* self)
{
    return g_displayHolder->engine;
}