#include <cstdio>

struct GossipNode {
    int f0;
    int f4;
    int f8;
    int fc;
};

struct GossipList {
    GossipNode* head;
    void Release(int node);
};

GossipList g_gossipList;
int g_gossipPtr;
int g_lastReleased = -999;

void GossipList::Release(int node) { g_lastReleased = node; }

void __fastcall CGameScriptInterface_ClearIsGossipForPlayer(void* self)
{
    (void)self;
    if (g_gossipPtr) {
        g_gossipList.Release(g_gossipList.head->f4);
        g_gossipList.head->f8 = (int)g_gossipList.head;
        g_gossipList.head->f4 = 0;
        g_gossipList.head->fc = (int)g_gossipList.head;
        g_gossipPtr = 0;
    }
}

int main()
{
    static GossipNode node;

    // Case 1: ptr null -> no-op
    g_gossipPtr = 0;
    g_gossipList.head = &node;
    node.f4 = 77;
    g_lastReleased = -999;
    CGameScriptInterface_ClearIsGossipForPlayer(0);
    if (g_lastReleased != -999 || node.f4 != 77) {
        std::printf("FAIL: null-ptr path modified state\n");
        return 1;
    }

    // Case 2: ptr non-null -> cleanup
    g_gossipPtr = 1;
    g_gossipList.head = &node;
    node.f4 = 123;
    node.f8 = 0;
    node.fc = 0;
    g_lastReleased = -999;
    CGameScriptInterface_ClearIsGossipForPlayer((void*)0x1234);
    if (g_lastReleased != 123) { std::printf("FAIL: Release arg=%d\n", g_lastReleased); return 1; }
    if (node.f4 != 0) { std::printf("FAIL: f4 not cleared\n"); return 1; }
    if (node.f8 != (int)&node) { std::printf("FAIL: f8\n"); return 1; }
    if (node.fc != (int)&node) { std::printf("FAIL: fc\n"); return 1; }
    if (g_gossipPtr != 0) { std::printf("FAIL: ptr not cleared\n"); return 1; }

    std::printf("CGameScriptInterface_008a7930_TEST PASS\n");
    return 0;
}