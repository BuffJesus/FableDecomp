class CCharString;
class C2DCoordI;

class CQuestManager
{
public:
    void SetQuestWorldMapOffset(
        const CCharString& questName,
        const C2DCoordI& offset);
};

class CGameScriptInterface
{
public:
    virtual void SetQuestWorldMapOffset(
        const CCharString& questName,
        const C2DCoordI& offset) const;
};

extern CQuestManager* CGSISetQuestWorldMapOffset_Manager;

void CGameScriptInterface::SetQuestWorldMapOffset(
    const CCharString& questName,
    const C2DCoordI& offset) const
{
    CGSISetQuestWorldMapOffset_Manager->SetQuestWorldMapOffset(
        questName,
        offset);
}
