// CQuestManager::RemoveAvailableQuestCardType  0x0088b8f0
// void __fastcall (CQuestManager*, CCharString*)

struct CCharString;

// The subobject at this+0x44 exposes a remove-by-name helper (call 0x88c840).
struct QuestCardSet {
    int pad[16];
    void Remove(CCharString* name);
};

struct CQuestManager {
    char pad0[0x44];       // 0x00
    QuestCardSet cards;    // 0x44
    char pad2[0x8c - (0x44 + sizeof(QuestCardSet))];
    unsigned char dirty;   // 0x8c

    void RemoveAvailableQuestCardType(CCharString* name);
};

void CQuestManager::RemoveAvailableQuestCardType(CCharString* name)
{
    this->cards.Remove(name);
    this->dirty = 1;
}