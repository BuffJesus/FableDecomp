#include <cstdio>

struct Node { int key; void* val; };

static void* g_mgr;
static void* g_ent;
static int g_check_ret;
static Node* g_lb_ret;

static void* __fastcall GetManagerA(void* self) { return g_mgr; }
static void* __fastcall GetPlayerEnt(void* mgr) { return g_ent; }
struct KeyArg { int* p; };
static Node* __fastcall LowerBound(void* tree, KeyArg k) { return g_lb_ret; }
static char  __fastcall CheckBlocking(void* obj) { return (char)g_check_ret; }

bool __fastcall CGameScriptInterface_IsPlayerCreatureBlocking(void* self)
{
    void* mgr = GetManagerA(*(void**)((char*)self + 0x14));
    void* e = GetPlayerEnt(mgr);
    if (e && !(*(unsigned char*)((char*)e + 0x91) & 1)
          && (*(unsigned int*)((char*)e + 0x24) & 0x10000))
    {
        char* tree = (char*)e + 0x44;
        int key = 0x30;
        KeyArg ka; ka.p = &key;
        Node* found = LowerBound(tree, ka);
        Node* end = *(Node**)(tree + 4);
        if (found == end || found->key > 0x30)
            found = end;
        if (CheckBlocking(*(void**)((char*)found + 4)))
            return true;
    }
    return false;
}

int main()
{
    static char entbuf[0x200];
    static char selfbuf[0x20];
    g_mgr = (void*)0x9999;
    g_ent = entbuf;
    *(void**)(selfbuf + 0x14) = (void*)0x1234;

    *(unsigned char*)(entbuf + 0x91) = 0;
    *(unsigned int*)(entbuf + 0x24) = 0x10000;

    static Node endnode; endnode.key = 0x30; endnode.val = (void*)0;
    static Node foundnode; foundnode.key = 0x20; foundnode.val = (void*)0xABCD;
    *(Node**)(entbuf + 0x44 + 4) = &endnode;

    g_lb_ret = &foundnode;
    g_check_ret = 1;
    bool r1 = CGameScriptInterface_IsPlayerCreatureBlocking(selfbuf);
    if (!r1) { std::printf("FAIL case1\n"); return 1; }

    static Node hifound; hifound.key = 0x40; hifound.val = (void*)0;
    g_lb_ret = &hifound;
    g_check_ret = 0;
    bool r2 = CGameScriptInterface_IsPlayerCreatureBlocking(selfbuf);
    if (r2) { std::printf("FAIL case2\n"); return 1; }

    *(unsigned char*)(entbuf + 0x91) = 1;
    bool r3 = CGameScriptInterface_IsPlayerCreatureBlocking(selfbuf);
    if (r3) { std::printf("FAIL case3\n"); return 1; }
    *(unsigned char*)(entbuf + 0x91) = 0;

    *(unsigned int*)(entbuf + 0x24) = 0;
    bool r4 = CGameScriptInterface_IsPlayerCreatureBlocking(selfbuf);
    if (r4) { std::printf("FAIL case4\n"); return 1; }

    std::printf("CGameScriptInterface_00899ba0_TEST PASS\n");
    return 0;
}