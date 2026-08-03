class CItem {
public:
    unsigned long field_0x0[3];
    unsigned long m_questPersistentFlag;
    void __fastcall SetQuestPersistentFlagImpl(bool bFlag);
};

void __fastcall CItem::SetQuestPersistentFlagImpl(bool bFlag)
{
    (void)bFlag;
    if (m_questPersistentFlag != 0)
    {
        m_questPersistentFlag = m_questPersistentFlag - 1;
    }
}