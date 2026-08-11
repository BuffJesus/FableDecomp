#include <cstdio>

struct CCharString;

struct QuestCardSet {
    int pad[16];
    void Remove(CCharString* name);
};

struct CQuestManager {
    char pad0[0x44];
    QuestCardSet cards;
    char pad2[0x8c - (0x44 + sizeof(QuestCardSet))];
    unsigned char dirty;

    void RemoveAvailableQuestCardType(CCharString* name);
};

static int g_removed = 0;
void QuestCardSet::Remove(CCharString* name) { g_removed++; }

int main()
{
    CQuestManager qm;
    qm.dirty = 0;
    g_removed = 0;
    qm.RemoveAvailableQuestCardType((CCharString*)0x1234);
    if (qm.dirty == 1 && g_removed == 1)
        printf("REMOVEQCT_OK\n");
    else
        printf("REMOVEQCT_FAIL\n");
    return (qm.dirty == 1 && g_removed == 1) ? 0 : 1;
}