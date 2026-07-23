extern int g_IsGossipForPlayerSize;

int __fastcall CGameScriptInterface_GetIsGossipForPlayerSize(void* self)
{
    (void)self;
    return g_IsGossipForPlayerSize;
}