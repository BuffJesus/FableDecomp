#include <stdio.h>

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

int main()
{
    CItem item;
    item.m_questPersistentFlag = 5;
    item.SetQuestPersistentFlagImpl(true);
    if (item.m_questPersistentFlag != 4)
    {
        printf("FAIL_DECREMENT\n");
        return 1;
    }

    CItem item2;
    item2.m_questPersistentFlag = 0;
    item2.SetQuestPersistentFlagImpl(false);
    if (item2.m_questPersistentFlag != 0)
    {
        printf("FAIL_ZERO_GUARD\n");
        return 1;
    }

    CItem item3;
    item3.m_questPersistentFlag = 1;
    item3.SetQuestPersistentFlagImpl(true);
    if (item3.m_questPersistentFlag != 0)
    {
        printf("FAIL_ONE_TO_ZERO\n");
        return 1;
    }

    printf("CITEM_SETQUESTPERSISTENTFLAG_OK\n");
    return 0;
}