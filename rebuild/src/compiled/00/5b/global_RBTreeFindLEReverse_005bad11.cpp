#pragma optimize("s",on)
struct RBNode {
    char pad[8];
    RBNode* left;
    RBNode* right;
    int key;
};
struct RBTree { RBNode* head; };
RBNode* __fastcall RBTree_FindLE_Reverse(RBTree* self, void* /*edx*/, int* pkey)
{
    RBNode* header = self->head;
    RBNode* result = header;
    RBNode* cur = *(RBNode**)((char*)header + 4);
    if (cur != 0) {
        int key = *pkey;
        do {
            if (cur->key >= key) {
                result = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        } while (cur != 0);
    }
    return result;
}