struct CCharString {
    void* p;
    CCharString(const CCharString& o);
};

struct CGlobal { char pad[0xb0]; CCharString name; };
extern CGlobal* g_obj;

struct CGameScriptInterface {
    char pad[0x1010];
    CCharString GetMostRecentValidUsedTargetName();
};

CCharString CGameScriptInterface::GetMostRecentValidUsedTargetName()
{
    return g_obj->name;
}