
class CCharString {
public:
    CCharString();
    CCharString(const CCharString& other);
    ~CCharString();
    char* m_pData;
};

class CGameScriptInterface {
public:
    void AddNewRumourToCategory(const CCharString& rumourCategory, const CCharString& rumourText) const;
};

void __stdcall CGameScriptInterface_AddRumourToCategoryEx(CCharString rumourCategory, CCharString rumourText);

void __fastcall CGameScriptInterface_AddNewRumourToCategory(CGameScriptInterface* self, void* /*edx*/, const CCharString& rumourCategory, const CCharString& rumourText)
{
    CGameScriptInterface_AddRumourToCategoryEx(rumourCategory, rumourText);
}