struct IPrimitive {
    struct VT { void (__fastcall *dtor)(IPrimitive*); void (__fastcall *slot1)(IPrimitive*); };
    VT* vt;
};

struct ListNode {
    ListNode* next;   // +0x0
    ListNode* prev;   // +0x4
    IPrimitive* payload; // +0x8
};

struct SubObj;
extern void __fastcall SubObj_Clear(SubObj* s);
extern int __cdecl GetSomeCounter();

struct CTCGraphicAppearance {
    char pad[0x38];
    SubObj* subptr;  // dummy at +0x38 region handled via cast
    ListNode* head;  // +0x3c  (pointer to sentinel node)
    char pad2[0x54 - 0x40];
    int counter;     // +0x54
    char flags;      // +0x58
    unsigned char state;      // +0x59
};

void __fastcall CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRange(CTCGraphicAppearance* self)
{
    if ((self->state & 0x80) == 0)
        return;
    {
        ListNode* n = self->head->next;
        while (n != self->head) {
            IPrimitive* p = n->payload;
            p->vt->slot1(p);
            n = n->next;
        }
        SubObj_Clear((SubObj*)((char*)self + 0x38));
        self->counter = GetSomeCounter() + 1;
        self->state &= 0x7f;
    }
}