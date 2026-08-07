#include "rebuild_abi.h"
// CWorldMap::PrepareForLoad @ 0x00cb0e90
// push esi;esi=ecx; eax=[esi+4]; push -1; push eax; call [import]; eax=[esi+0xc]; ret
extern void (FABLE_STDCALL *g_import_cb0e90)(void *a, int b);
struct CWorldMap { char pad4[4]; void *m_h; char pad[4]; int m_result; int PrepareForLoad(); };
int CWorldMap::PrepareForLoad(){ g_import_cb0e90(m_h, -1); return m_result; }