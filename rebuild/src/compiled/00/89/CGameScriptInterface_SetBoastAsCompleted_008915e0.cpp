class CCharString
{
public:
    explicit CCharString(unsigned long value)
        : m_Value(value)
    {
    }

    CCharString(const CCharString& other);

    unsigned long m_Value;
};

class CQuestManager
{
public:
    void SetBoastAsCompleted(
        long boastId,
        CCharString boastName);
};

class CGameScriptInterface
{
public:
    virtual void SetBoastAsCompleted(
        long boastId,
        const CCharString& boastName) const;
};

extern CQuestManager* CGSISetBoastAsCompleted_Manager;

void CGameScriptInterface::SetBoastAsCompleted(
    long boastId,
    const CCharString& boastName) const
{
    CQuestManager* const questManager =
        CGSISetBoastAsCompleted_Manager;
    questManager->SetBoastAsCompleted(
        boastId,
        boastName);
}
