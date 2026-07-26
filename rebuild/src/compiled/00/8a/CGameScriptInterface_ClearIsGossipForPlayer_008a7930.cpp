struct GossipNode {
    int f0;
    int f4;
    int f8;
    int fc;
};

struct GossipList {
    GossipNode* head;   // at +0
    void Release(int node);
};

extern GossipList g_gossipList;   // 0x13bae38
extern int g_gossipPtr;           // 0x13bae3c

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