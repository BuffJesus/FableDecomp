#pragma optimize("s",on)
struct Node {
    Node* next;   // +0x0
    Node* prev;   // +0x4
};
struct Tree {
    Node* sentinel;  // +0x0 : pointer to the list sentinel node
};

extern void __cdecl FreeNode(Node* p);

void __fastcall CGuiControlTreePane_SortTreeRecursively(Tree* self)
{
    Node* head = self->sentinel;
    Node* n = head->next;
    if (n != head) {
        do {
            Node* cur = n;
            n = n->next;
            FreeNode(cur);
        } while (n != self->sentinel);
    }
    self->sentinel->next = self->sentinel;
    self->sentinel->prev = self->sentinel;
}