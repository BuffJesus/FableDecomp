class CWorld
{
public:
    void AutoSaveQuestStart();
};

class CGameScriptInterface
{
public:
    void* m_pVtbl;
    CWorld* m_pWorld;
    void AutoSaveQuestStart() const;
};

void CGameScriptInterface::AutoSaveQuestStart() const
{
    m_pWorld->AutoSaveQuestStart();
}