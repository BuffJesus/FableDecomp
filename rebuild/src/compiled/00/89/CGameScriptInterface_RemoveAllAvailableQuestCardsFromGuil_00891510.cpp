struct CGuild;
extern CGuild* g_pGuild;
extern void __fastcall Guild_RemoveAllAvailableQuestCards(CGuild* self);

void __fastcall CGameScriptInterface_RemoveAllAvailableQuestCardsFromGuild(void* self)
{
    (void)self;
    Guild_RemoveAllAvailableQuestCards(g_pGuild);
}