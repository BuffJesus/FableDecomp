struct CCharString {
    unsigned int value;
    CCharString(const CCharString& other);
};

extern void __stdcall CGameScriptInterface_FindOrCreateRumourCategory(CCharString category);

struct CGameScriptInterface {
    void* vt;
    void AddRumourCategory(const CCharString& category) const;
};

void CGameScriptInterface::AddRumourCategory(const CCharString& category) const
{
    CGameScriptInterface_FindOrCreateRumourCategory(category);
}