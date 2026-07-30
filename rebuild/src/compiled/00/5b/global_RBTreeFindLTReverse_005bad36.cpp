#pragma optimize("s",on)
struct Node {
    unsigned int pad0;
    unsigned int pad4;
    Node* left;          // +8
    Node* right;         // +0xc
    int key;             // +0x10
};
struct Tree {
    Node* head;          // +0
};

Node* __fastcall RBTree_FindLT_Reverse(Tree* self, void* /*edx*/, int* pkey)
{
    Node* result = self->head;                      // eax = [ecx]
    Node* cur = *(Node**)((char*)result + 4);       // ecx = [eax+4]
    while (cur) {
        if (*pkey < cur->key) {
            result = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return result;
}