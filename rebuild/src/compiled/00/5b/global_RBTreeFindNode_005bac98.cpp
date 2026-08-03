#pragma optimize("s",on)
// RBTree_FindNode @ 0x005bac98
// __fastcall lower_bound-style walk over an STL red-black tree keyed by int.
// this (ecx) -> tree; [this] -> head/nil node; head->child[+4] = root.
// Node: +0x8 = left, +0xc = right, +0x10 = int key.
// arg: const int* pKey (dereferenced once), passed on stack (ret 4).

struct Node {
    Node* pad0;      // +0x0
    Node* pad4;      // +0x4  (on head node this is _Root)
    Node* left;      // +0x8
    Node* right;     // +0xc
    int   key;       // +0x10
};

struct Tree {
    Node* head;      // +0x0
    Node* RBTree_FindNode(const int* pKey);
};

Node* Tree::RBTree_FindNode(const int* pKey)
{
    Node* result = this->head;          // eax = [ecx]
    Node* node   = result->pad4;        // ecx = [eax+4] = root
    if (node != 0) {
        int k = *pKey;                  // edx = [[esp+4]]
        do {
            if (node->key >= k) {       // cmp [ecx+0x10],edx ; jl -> right (fallthrough here)
                result = node;          // remember candidate
                node = node->left;
            } else {
                node = node->right;
            }
        } while (node != 0);
    }
    return result;
}