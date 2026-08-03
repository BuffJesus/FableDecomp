#pragma optimize("s",on)

struct Node {
    Node* m0;
    Node* parent;
    Node* left;
    Node* right;
    int   key;
};
struct Tree {
    Node* head;
};

Node* __fastcall BinarySearchTree_FindByIntKey(Tree* self, void* /*edx*/, int* pkey)
{
    Node* result = self->head;
    Node* cur = result->parent;
    while (cur) {
        int k = *pkey;
        if (cur->key >= k) {
            result = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return result;
}