// CGuiControlTreePane::SortTreeRecursively @ 004407b0 (__fastcall, void)
// Clears a circular doubly-linked list: destroys each node's subobject and frees it.
struct Node {
    Node* next; // +0x0
    Node* prev; // +0x4
    char pad[0x10]; // +0x8 .. +0x17
    int sub; // +0x18 : subobject destroyed via thiscall
};
struct CGuiControlTreePane {
    Node* head; // +0x0 pointer to sentinel head node
};
extern "C" void __fastcall NodeSub_Dtor(int* self); // 0x55ad60, thiscall on node+0x18
extern "C" void FreeNode(Node* n);                 // 0x7be264, cdecl

void __fastcall CGuiControlTreePane_SortTreeRecursively(CGuiControlTreePane* self)
{
    Node* head = self->head;
    Node* cur = head->next;
    if (cur != head) {
        do {
            Node* node = cur;
            cur = cur->next;
            NodeSub_Dtor(&node->sub);
            FreeNode(node);
        } while (cur != self->head);
    }
    head = self->head;
    head->next = head;
    head = self->head;
    head->prev = head;
}