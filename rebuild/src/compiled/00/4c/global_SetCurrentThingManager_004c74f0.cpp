// SetCurrentThingManager @ 0x004c74f0
// Retail: mov [g_CurrentThingManager], ecx ; ret
// ecx-passed single pointer -> stored into a global. __fastcall single arg lands in ecx.
extern void *g_CurrentThingManager;

void __fastcall SetCurrentThingManager(void *pManager)
{
    g_CurrentThingManager = pManager;
}