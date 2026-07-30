// SetCurrentThingManager @ 0x004bbfc0
// mov dword ptr [g_CurrentThingManager], ecx ; ret
// __fastcall: single pointer arg arrives in ecx, stored to a global.
extern void *g_CurrentThingManager;

void __fastcall SetCurrentThingManager(void *mgr)
{
    g_CurrentThingManager = mgr;
}