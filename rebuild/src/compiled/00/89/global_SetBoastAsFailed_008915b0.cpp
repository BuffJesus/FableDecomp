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
    virtual void SetBoastAsFailed(
        long boastId,
        const CCharString& boastName) const;
};

extern CQuestManager* CGSISetBoastAsFailed_Manager;

void CGameScriptInterface::SetBoastAsFailed(
    long boastId,
    const CCharString& boastName) const
{
    CQuestManager* const questManager =
        CGSISetBoastAsFailed_Manager;
    questManager->SetBoastAsCompleted(
        boastId,
        boastName);
}