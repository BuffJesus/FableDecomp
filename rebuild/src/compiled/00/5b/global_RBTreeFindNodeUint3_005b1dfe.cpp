#pragma optimize("s",on)
struct Node {
    unsigned int pad0;   // +0
    unsigned int pad4;   // +4
    Node* left;          // +8
    Node* right;         // +0xc
    unsigned int key;    // +0x10
};
struct Tree {
    Node* head;          // +0
};

// __fastcall: self=ecx, dummy edx, pkey on the stack -> ret 4.
Node* __fastcall RBTree_FindNode_Uint3(Tree* self, void* /*edx*/, unsigned int* pkey)
{
    Node* head = self->head;
    Node* cur = *(Node**)((char*)head + 4);
    Node* bound = head;
    if (cur) {
        do {
            if (cur->key >= *pkey) {
                bound = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        } while (cur);
    }
    return bound;
}