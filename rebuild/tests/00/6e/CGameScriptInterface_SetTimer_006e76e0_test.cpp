#include <cstdio>

struct Node { int f0; int f4; int f8; int fc; int f10; int val14; };
struct Tree {
    Node* head; // +0x0
    Node* g_found;
    void Find(Node** out, int* key) { *out = g_found; }
};

struct CGameScriptInterface {
    unsigned char _pad[0x3c];
    Tree tree; // +0x3c
};

void __fastcall CGameScriptInterface_SetTimer(CGameScriptInterface* self, void* edx, int key, int value)
{
    Tree* t = &self->tree;
    Node* it;
    t->Find(&it, &key);
    if (it != t->head) {
        it->val14 = value;
    }
}

int main()
{
    CGameScriptInterface obj;
    Node headNode; Node other;
    obj.tree.head = &headNode;

    // case 1: found node differs from head -> write value
    other.val14 = 0;
    obj.tree.g_found = &other;
    CGameScriptInterface_SetTimer(&obj, 0, 7, 1234);
    if (other.val14 != 1234) { std::printf("FAIL write\n"); return 1; }

    // case 2: found == head -> no write
    headNode.val14 = 55;
    obj.tree.g_found = &headNode;
    CGameScriptInterface_SetTimer(&obj, 0, 7, 9999);
    if (headNode.val14 != 55) { std::printf("FAIL nowrite\n"); return 1; }

    std::printf("CGameScriptInterface_006e76e0_TEST PASS\n");
    return 0;
}