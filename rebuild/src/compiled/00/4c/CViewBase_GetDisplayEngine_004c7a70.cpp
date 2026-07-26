struct CDisplayEngine;
struct GlobalRoot { char pad[0x1c]; CDisplayEngine* engine; };
extern GlobalRoot* g_root;
struct CViewBase {};
CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(CViewBase* self)
{
    (void)self;
    return g_root->engine;
}