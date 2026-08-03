struct CCharStringData;

class CCharString
{
public:
    CCharStringData* m_pData;

    CCharString(const CCharString& other);
    ~CCharString();
};

class CGameScriptInterface
{
public:
    virtual void AddGossipFactionToCategory(
        const CCharString& category,
        const CCharString& faction) const;
};

void __stdcall AddGossipFactionToCategory_Impl(
    CCharString category,
    CCharString faction);

void CGameScriptInterface::AddGossipFactionToCategory(
    const CCharString& category,
    const CCharString& faction) const
{
    AddGossipFactionToCategory_Impl(category, faction);
}