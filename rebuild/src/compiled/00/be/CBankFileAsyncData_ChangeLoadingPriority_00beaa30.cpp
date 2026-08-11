// CBankFileAsyncData::ChangeLoadingPriority @ 0x00beaa30
// void __fastcall(CBankFileAsyncData*, EThreadedFilePriority)

typedef int EThreadedFilePriority;

struct CBankFileAsyncData;

struct AsyncMgr {
    // callee at 0x00bd9ab0 : __fastcall(AsyncMgr*, void* item, CBankFileAsyncData* owner)
    void Reprioritise(void* item, CBankFileAsyncData* owner);
};

struct AsyncRoot {
    char pad[0x6b0];
    AsyncMgr* mgr;   // +0x6b0
};

extern AsyncRoot* g_asyncRoot;   // 0x1436ea8

struct CBankFileAsyncData {
    char pad[0x10];
    void* item;      // +0x10
    void ChangeLoadingPriority();
};

void CBankFileAsyncData::ChangeLoadingPriority()
{
    void* it = this->item;
    if (it)
    {
        g_asyncRoot->mgr->Reprioritise(it, this);
    }
}