#pragma optimize("s",on)
// RBTree_FindNode_Dup @ 0x005bacec
// __fastcall (this=ecx) lower_bound-style walk over an STL red-black tree keyed by int.
// [this] -> head/nil node; head->_Parent (+0x4) = root.
// Node: +0x8 = left, +0xc = right, +0x10 = int key.  arg: const int* pKey (deref once), ret 4.
struct Node {
    Node* pad0;   // +0x0
    Node* parent; // +0x4
    Node* left;   // +0x8
    Node* right;  // +0xc
    int   key;    // +0x10
};
struct Tree { Node* head; };

Node* __fastcall RBTree_FindNode_Dup(Tree* self, int /*edx*/, const int* pKey)
{
    Node* result = self->head;      // mov eax,[ecx]
    Node* node   = result->parent;  // mov ecx,[eax+4]
    if (node != 0) {                // test ecx,ecx ; je
        int k = *pKey;              // mov edx,[esp+4] ; mov edx,[edx]
        do {
            if (node->key >= k) {   // cmp [ecx+0x10],edx ; jl -> else(right)
                result = node;      // mov eax,ecx
                node = node->left;  // mov ecx,[ecx+0x8]
            } else {
                node = node->right; // mov ecx,[ecx+0xc]
            }
        } while (node != 0);        // test ecx,ecx ; jne
    }
    return result;                  // eax
}