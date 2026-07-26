struct Node { int f0; int f4; int f8; int fc; int f10; int val14; };
struct Tree {
    Node* head; // +0x0 == self+0x3c
    void Find(Node** out, int* key);
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