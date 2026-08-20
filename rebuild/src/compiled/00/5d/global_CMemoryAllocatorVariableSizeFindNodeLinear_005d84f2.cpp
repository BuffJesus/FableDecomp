#pragma optimize("s",on)
struct RBNode {
    unsigned int _pad0;   // 0x00
    RBNode* parent;       // 0x04
    RBNode* left;         // 0x08
    RBNode* right;        // 0x0C
    unsigned int key;     // 0x10
};

struct RBTree {
    RBNode* header;       // 0x00
};

// __fastcall: ecx=self, edx=dummy, key on stack -> ret 4
RBNode* __fastcall UInt_RBTreeFindGreaterOrEqual(RBTree* self, int /*edx*/, unsigned int* key)
{
    RBNode* result = self->header;
    RBNode* node = result->parent;
    while (node != 0) {
        if (node->key >= *key) {
            result = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return result;
}