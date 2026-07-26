#include <cstdio>

struct CGuild { int flag; };
static CGuild g_obj = { 0 };
CGuild* g_pGuild = &g_obj;
static CGuild* g_seen = 0;

void __fastcall Guild_RemoveAllAvailableQuestCards(CGuild* self)
{
    g_seen = self;
    self->flag = 1;
}

void __fastcall CGameScriptInterface_RemoveAllAvailableQuestCardsFromGuild(void* self)
{
    (void)self;
    Guild_RemoveAllAvailableQuestCards(g_pGuild);
}

int main()
{
    CGameScriptInterface_RemoveAllAvailableQuestCardsFromGuild(0);
    if (g_seen == &g_obj && g_obj.flag == 1) {
        std::printf("CGameScriptInterface_00891510_TEST PASS\n");
        return 0;
    }
    std::printf("CGameScriptInterface_00891510_TEST FAIL\n");
    return 1;
}