struct CCharString
{
    char* p;
    CCharString(const CCharString& other);
    ~CCharString();
};

void __stdcall CGameScriptInterface_AddGossipVillageEx(CCharString gossipVillage, CCharString gossipText);

class CGameScriptInterface
{
public:
    virtual void AddGossipVillage(const CCharString& gossipVillage, const CCharString& gossipText) const;
};

void CGameScriptInterface::AddGossipVillage(const CCharString& gossipVillage, const CCharString& gossipText) const
{
    CGameScriptInterface_AddGossipVillageEx(gossipVillage, gossipText);
}