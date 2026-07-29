class CCharString;
class CRegion;
class CWorldMap;

class CWorld
{
public:
    virtual void Slot00() {}
    virtual void Slot04() {}
    virtual void Slot08() {}
    virtual void Slot0C() {}
    virtual void Slot10() {}
    virtual void Slot14() {}
    virtual void Slot18() {}
    virtual void Slot1C() {}
    virtual void Slot20() {}
    virtual void Slot24() {}
    virtual void Slot28() {}
    virtual void Slot2C() {}
    virtual void Slot30() {}
    virtual CWorldMap* GetWorldMap() = 0;
};

class CWorldMap
{
public:
    long GetRegionNumberFromName(
        const CCharString& regionName);
    CRegion& GetRegion(long regionNumber);
};

class CQuestManager
{
public:
    void AddQuestRegion(
        const CCharString& questName,
        const CRegion* region);
};

struct CGSIAddQuestRegion_Interface
{
    unsigned char m_Unused[0x04];
    CWorld* m_World;
};

class CGameScriptInterface
{
public:
    virtual void AddQuestRegion(
        const CCharString& questName,
        const CCharString& regionName) const;
};

extern CQuestManager* CGSIAddQuestRegion_QuestManager;

void CGameScriptInterface::AddQuestRegion(
    const CCharString& questName,
    const CCharString& regionName) const
{
    const long regionNumber =
        reinterpret_cast<const CGSIAddQuestRegion_Interface*>(
            this)->m_World->GetWorldMap()
            ->GetRegionNumberFromName(regionName);
    if (regionNumber > 0)
    {
        CRegion& region =
            reinterpret_cast<const CGSIAddQuestRegion_Interface*>(
                this)->m_World->GetWorldMap()
                ->GetRegion(regionNumber);
        CGSIAddQuestRegion_QuestManager->AddQuestRegion(
            questName,
            &region);
    }
}
