class CCharString
{
public:
    CCharString(const char* text, long length);
    ~CCharString();

private:
    const char* m_Text;
};

class CGameScriptInterface
{
public:
    virtual void AddQuestCard(
        const CCharString& questName,
        const CCharString& questDescription,
        bool addToFront,
        bool suppressNotification) const;

    void AddQuestCardEntry(
        const CCharString& questName,
        const CCharString& questDescription,
        bool addToFront);
};

typedef int (CGameScriptInterface::*CGSIAddQuestCard_GetValue)()
    const;
typedef void (CGameScriptInterface::*CGSIAddQuestCard_Notify)(
    const CCharString& message,
    const CCharString& orbName,
    bool immediate,
    bool unknown) const;

struct CGSIAddQuestCard_VTable
{
    unsigned char m_Unused000[0x448];
    CGSIAddQuestCard_Notify m_Notify;
    unsigned char m_Unused44C[0xA34 - 0x44C];
    CGSIAddQuestCard_GetValue m_GetValue;
};

extern long CGSIAddQuestCard_CurrentValue;
extern CGameScriptInterface* CGSIAddQuestCard_Manager;

void CGameScriptInterface::AddQuestCard(
    const CCharString& questName,
    const CCharString& questDescription,
    bool addToFront,
    bool suppressNotification) const
{
    const long currentValue =
        (this->*(*reinterpret_cast<
            const CGSIAddQuestCard_VTable* const*>(
                this))->m_GetValue)();

    if (CGSIAddQuestCard_CurrentValue != currentValue)
    {
        if (!suppressNotification)
        {
            CCharString orbName(
                "HUD_ORB_QUEST_CORE",
                -1);
            CCharString message(
                "TEXT_QST_078_GM_MSG_NEW_QUEST",
                -1);
            (this->*(*reinterpret_cast<
                const CGSIAddQuestCard_VTable* const*>(
                    this))->m_Notify)(
                message,
                orbName,
                true,
                false);
        }

        CGSIAddQuestCard_CurrentValue =
            (this->*(*reinterpret_cast<
                const CGSIAddQuestCard_VTable* const*>(
                    this))->m_GetValue)();
    }

    CGSIAddQuestCard_Manager->AddQuestCardEntry(
        questName,
        questDescription,
        addToFront);
}
