#include <cstdio>

static int g_pre = 0, g_post = 0, g_handle = 0;
static void* g_find_ret = 0;
static int g_seen_key = -1;

struct CGSI {
    void GSI_PreMsg() { g_pre = 1; }
    void GSI_PostMsg() { g_post = 1; }
};
struct CMgr {
    void* Find(int* pKey) { g_seen_key = *pKey; return g_find_ret; }
    void Handle(void* pObj) { g_handle = 1; }
};

bool __fastcall CGameScriptInterface_MsgOnChestOpeningCancelled(CGSI* self)
{
    self->GSI_PreMsg();
    self->GSI_PostMsg();
    CMgr* mgr = *(CMgr**)((char*)(*(void**)((char*)self + 4)) + 0x60);
    int key = 0x48;
    void* obj = mgr->Find(&key);
    if (obj) {
        mgr->Handle(obj);
        return true;
    }
    return false;
}

int main()
{
    // Build fake object graph
    char mgr[0x100];
    char inner[0x100];
    char self[0x10];
    *(void**)(inner + 0x60) = mgr;      // inner[+0x60] = mgr
    *(void**)(self + 4) = inner;         // self[+4] = inner
    CGSI* pself = (CGSI*)self;

    // Case 1: find returns non-null -> true, handle called
    g_find_ret = (void*)0xDEADBEEF;
    g_pre = g_post = g_handle = 0; g_seen_key = -1;
    bool r1 = CGameScriptInterface_MsgOnChestOpeningCancelled(pself);
    if (!(r1 && g_pre && g_post && g_handle && g_seen_key == 0x48)) {
        std::printf("FAIL case1 r=%d pre=%d post=%d handle=%d key=%d\n", r1, g_pre, g_post, g_handle, g_seen_key);
        return 1;
    }

    // Case 2: find returns null -> false, handle not called
    g_find_ret = 0;
    g_pre = g_post = g_handle = 0; g_seen_key = -1;
    bool r2 = CGameScriptInterface_MsgOnChestOpeningCancelled(pself);
    if (!(!r2 && g_pre && g_post && !g_handle && g_seen_key == 0x48)) {
        std::printf("FAIL case2 r=%d pre=%d post=%d handle=%d key=%d\n", r2, g_pre, g_post, g_handle, g_seen_key);
        return 1;
    }

    std::printf("CGameScriptInterface_008942c0_TEST PASS\n");
    return 0;
}