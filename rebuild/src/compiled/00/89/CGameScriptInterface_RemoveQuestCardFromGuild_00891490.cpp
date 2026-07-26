// CGameScriptInterface::RemoveQuestCardFromGuild @ 00891490
struct Str {
    void* p;
    void ctor(int v);
    char check(int c);
    void dtor();
};
struct Guild {
    void* vt;
    char pad[0x100];
    int  map(int id);
    void remove(Str* s);
};
extern Guild* g_guild;   // ds:0x13b89fc

void __stdcall CGameScriptInterface_RemoveQuestCardFromGuild(int questId)
{
    int v = g_guild->map(questId);
    Str tmp;
    tmp.ctor(v);
    if (tmp.check(0x122d70e)) {
        g_guild->remove(&tmp);
    }
    tmp.dtor();
}