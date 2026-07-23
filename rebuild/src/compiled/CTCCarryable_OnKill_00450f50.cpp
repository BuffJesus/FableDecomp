#pragma optimize("s",on)
struct Node {
    void* field0;    // +0x0
    void* next;      // +0x4
    void* field8;    // +0x8
    void* fieldC;    // +0xC
};

struct CTCCarryable {
    Node* obj;       // +0x0
    int   count;     // +0x4
};

extern void __stdcall ReleaseThing(void* p);

void __fastcall CTCCarryable_OnKill(CTCCarryable* self)
{
    if (self->count != 0) {
        ReleaseThing(self->obj->next);
        self->obj->field8 = self->obj;
        self->obj->next = 0;
        self->obj->fieldC = self->obj;
        self->count = 0;
    }
}