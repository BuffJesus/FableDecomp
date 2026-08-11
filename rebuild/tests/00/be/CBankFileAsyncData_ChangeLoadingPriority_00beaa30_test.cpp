#include <cstdio>

typedef int EThreadedFilePriority;

struct CBankFileAsyncData;

struct AsyncMgr {
    void Reprioritise(void* item, CBankFileAsyncData* owner);
};

struct AsyncRoot {
    char pad[0x6b0];
    AsyncMgr* mgr;
};

AsyncRoot* g_asyncRoot;

struct CBankFileAsyncData {
    char pad[0x10];
    void* item;
    void ChangeLoadingPriority();
};

// captured call
static void* g_seenItem;
static CBankFileAsyncData* g_seenOwner;
static int g_calls;

void AsyncMgr::Reprioritise(void* item, CBankFileAsyncData* owner)
{
    g_seenItem = item;
    g_seenOwner = owner;
    ++g_calls;
}

void CBankFileAsyncData::ChangeLoadingPriority()
{
    void* it = this->item;
    if (it)
    {
        g_asyncRoot->mgr->Reprioritise(it, this);
    }
}

int main()
{
    AsyncMgr mgr;
    static char rootBuf[sizeof(AsyncRoot)];
    AsyncRoot* root = (AsyncRoot*)rootBuf;
    root->mgr = &mgr;
    g_asyncRoot = root;

    // Case 1: item != null -> forwards (item, this)
    CBankFileAsyncData a;
    int dummy = 0x1234;
    a.item = &dummy;
    g_calls = 0; g_seenItem = 0; g_seenOwner = 0;
    a.ChangeLoadingPriority();
    bool ok1 = (g_calls == 1) && (g_seenItem == &dummy) && (g_seenOwner == &a);

    // Case 2: item == null -> no call
    CBankFileAsyncData b;
    b.item = 0;
    g_calls = 0;
    b.ChangeLoadingPriority();
    bool ok2 = (g_calls == 0);

    if (ok1 && ok2)
        printf("CHANGELOADINGPRIORITY_OK\n");
    else
        printf("FAIL c1=%d c2=%d\n", ok1, ok2);
    return (ok1 && ok2) ? 0 : 1;
}