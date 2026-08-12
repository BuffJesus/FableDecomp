// CQuestManager::RemoveAllBoasts  @ 004aff80

struct BoastList {
    int data;
    void Clear();   // 0x4b8c00
};

int MakeDefault();  // 0x49d870

struct CQuestManager {
    char pad[0x64];
    int  field64;    // +0x64
    BoastList a;     // +0x68
    BoastList b;     // +0x6c
    void RemoveAllBoasts();
};

void CQuestManager::RemoveAllBoasts()
{
    this->a.Clear();
    this->b.Clear();
    this->field64 = MakeDefault();
}