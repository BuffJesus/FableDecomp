#pragma optimize("s",on)
extern void* g_pGameDefinitionManager;

bool __fastcall CGameDefinitionManager_IsAllocated()
{
    return g_pGameDefinitionManager != 0;
}