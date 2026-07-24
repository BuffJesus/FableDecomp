#include <cstdio>

struct CCharString {
    char* p;
    CCharString(const char* s, int len) { p = (char*)s; (void)len; }
    CCharString(const CCharString& o) { p = o.p; }
};

struct QuestObj {
    char _pad[0x30];
    CCharString name; // +0x30
    QuestObj(const char* s) : name(s, -1) {}
};

struct GameGlobal {
    char _pad[0x88];
    QuestObj* active; // +0x88
};

GameGlobal* g_ptr;

struct CGameScriptInterface {
    void* vt;
    CCharString GetActiveQuestName();
};

CCharString CGameScriptInterface::GetActiveQuestName()
{
    QuestObj* q = g_ptr->active;
    if (q)
        return q->name;
    return CCharString("", -1);
}

int main() {
    GameGlobal g; g.active = 0;
    g_ptr = &g;
    CGameScriptInterface self;
    CCharString r1 = self.GetActiveQuestName();
    if (r1.p == 0 || r1.p[0] != 0) { std::printf("FAIL null-branch not empty\n"); return 1; }

    static QuestObj q("Wasp Menace");
    g.active = &q;
    CCharString r2 = self.GetActiveQuestName();
    if (std::printf("%s", r2.p) < 0) { std::printf("FAIL\n"); return 1; }
    std::printf("\nCGameScriptInterface_00891880_TEST PASS\n");
    return 0;
}