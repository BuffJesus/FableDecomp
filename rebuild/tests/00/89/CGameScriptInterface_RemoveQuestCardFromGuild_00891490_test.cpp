#include <cstdio>

struct Str { char buf[0x20]; };
struct Guild { void* vt; char pad[0x100]; };

static Guild g_guild_obj;
Guild* g_guild = &g_guild_obj;

static int g_mapArg = -1;
static int g_checkArg = 0;
static int g_ctorVal = 0;
static bool g_removed = false;
static bool g_dtored = false;
static bool g_returnCheck = true;

int __fastcall Guild_map(Guild* g, int id) { g_mapArg = id; return id + 100; }
void __fastcall Str_ctor(Str* s, int v) { g_ctorVal = v; s->buf[0] = 0; }
char __fastcall Str_check(Str* s, int c) { g_checkArg = c; return g_returnCheck ? 1 : 0; }
void __fastcall Guild_remove(Guild* g, Str* s) { g_removed = true; }
void __fastcall Str_dtor(Str* s) { g_dtored = true; }

void CGameScriptInterface_RemoveQuestCardFromGuild(int questId)
{
    int v = Guild_map(g_guild, questId);
    Str tmp;
    Str_ctor(&tmp, v);
    if (Str_check(&tmp, 0x122d70e)) {
        Guild_remove(g_guild, &tmp);
    }
    Str_dtor(&tmp);
}

int main()
{
    g_returnCheck = true;
    CGameScriptInterface_RemoveQuestCardFromGuild(42);
    if (g_mapArg != 42) { std::printf("FAIL mapArg=%d\n", g_mapArg); return 1; }
    if (g_ctorVal != 142) { std::printf("FAIL ctorVal=%d\n", g_ctorVal); return 1; }
    if (g_checkArg != 0x122d70e) { std::printf("FAIL checkArg=%x\n", g_checkArg); return 1; }
    if (!g_removed) { std::printf("FAIL not removed\n"); return 1; }
    if (!g_dtored) { std::printf("FAIL not dtored\n"); return 1; }

    g_removed = false; g_dtored = false; g_returnCheck = false;
    CGameScriptInterface_RemoveQuestCardFromGuild(7);
    if (g_removed) { std::printf("FAIL removed when check false\n"); return 1; }
    if (!g_dtored) { std::printf("FAIL not dtored on false path\n"); return 1; }

    std::printf("CGameScriptInterface_00891490_TEST PASS\n");
    return 0;
}