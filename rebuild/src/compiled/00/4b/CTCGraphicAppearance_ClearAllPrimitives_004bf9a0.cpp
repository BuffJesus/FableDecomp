
struct Prim {
    virtual void slot0();
    virtual void slot1();
};

struct ListNode {
    ListNode* next;
    ListNode* prev;
    Prim* obj;
};

struct MemberAt38 {
    int dummy;
    void Clear();
};

extern int GetSomeCounter();

struct CTCGraphicAppearance {
    char pad[0x38];
    MemberAt38 m38;
    ListNode* head;
    char pad2[0x54 - 0x40];
    int counter;
};

void __fastcall CTCGraphicAppearance_ClearAllPrimitives(CTCGraphicAppearance* self)
{
    ListNode* node = self->head->next;
    while (node != self->head) {
        node->obj->slot1();
        node = node->next;
    }
    self->m38.Clear();
    self->counter = GetSomeCounter() + 1;
}