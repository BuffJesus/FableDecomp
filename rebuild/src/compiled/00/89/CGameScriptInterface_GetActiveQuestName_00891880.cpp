#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    char* p;
    CCharString(const char* s, int len);
    CCharString(const CCharString& o);
};

struct QuestObj {
    char _pad[0x30];
    CCharString name; // +0x30
};

struct GameGlobal {
    char _pad[0x88];
    QuestObj* active; // +0x88
};

extern GameGlobal* g_ptr; // ds:0x13b89fc

struct CGameScriptInterface_Methods : CGameScriptInterface {
    CCharString GetActiveQuestName();
};

CCharString CGameScriptInterface_Methods::GetActiveQuestName()
{
    QuestObj* q = g_ptr->active;
    if (q)
        return q->name;
    return CCharString("", -1);
}