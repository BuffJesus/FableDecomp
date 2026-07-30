
struct QuestObj { char pad[0x18]; unsigned char flag; }; // flag at +0x18

struct CItem {
    char pad0[0x8];
    QuestObj* pQuest;   // +0x8
    void SetQuestPersistentFlag(bool flag);
};

void CItem::SetQuestPersistentFlag(bool flag)
{
    QuestObj* p = this->pQuest;
    if (p)
        p->flag = (unsigned char)flag;
}