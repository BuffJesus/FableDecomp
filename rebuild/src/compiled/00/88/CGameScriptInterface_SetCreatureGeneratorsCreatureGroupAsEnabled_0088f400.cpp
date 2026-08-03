enum ECreatureGroup { ECreatureGroup_Dummy };

class CWorld
{
public:
    void __fastcall SetCreatureGenerationGroupAsEnabled(
        ECreatureGroup creatureGroup,
        bool enabled);
};

class CGameScriptInterface
{
public:
    virtual void __fastcall SetCreatureGeneratorsCreatureGroupAsEnabled(
        ECreatureGroup creatureGroup,
        bool enabled) const;
};

struct CGameScriptInterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CWorld* m_pWorld; /* 0x04 */
};

void __fastcall CGameScriptInterface::SetCreatureGeneratorsCreatureGroupAsEnabled(
    ECreatureGroup creatureGroup,
    bool enabled) const
{
    reinterpret_cast<const CGameScriptInterfaceOverlay*>(this)->m_pWorld
        ->SetCreatureGenerationGroupAsEnabled(creatureGroup, enabled);
}