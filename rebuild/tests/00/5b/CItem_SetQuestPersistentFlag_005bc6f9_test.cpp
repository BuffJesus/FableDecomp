
#include <stdio.h>

struct QuestObj { char pad[0x18]; unsigned char flag; };
struct CItem {
    char pad0[0x8];
    QuestObj* pQuest;
    void SetQuestPersistentFlag(bool flag);
};
void CItem::SetQuestPersistentFlag(bool flag)
{
    QuestObj* p = this->pQuest;
    if (p)
        p->flag = (unsigned char)flag;
}

int main()
{
    // Case 1: non-null pQuest -> flag byte set
    QuestObj q;
    q.flag = 0xAA;
    CItem it;
    it.pQuest = &q;
    it.SetQuestPersistentFlag(true);
    if (q.flag != 1) { printf("FAIL set true got %d\n", (int)q.flag); return 1; }
    it.SetQuestPersistentFlag(false);
    if (q.flag != 0) { printf("FAIL set false got %d\n", (int)q.flag); return 1; }

    // Case 2: null pQuest -> no crash, no-op
    CItem it2;
    it2.pQuest = 0;
    it2.SetQuestPersistentFlag(true); // must not crash

    printf("OK_0x005bc6f9\n");
    return 0;
}