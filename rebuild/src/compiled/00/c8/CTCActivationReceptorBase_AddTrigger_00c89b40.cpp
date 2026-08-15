// Byte-exact reconstruction of CTCActivationReceptorBase::AddTrigger @ 0x00c89b40
struct CTCActivationTrigger;

struct CTCActivationReceptorBase {
    char pad0[0x18];
    int sub18;          // +0x18
    char pad1c[0x04];   // +0x1c
    void* field20;      // +0x20
};

// The global receiver (this for the forwarded thiscall). Its method @ 0xc763e0
// takes four stack args (this in ecx).
struct GTarget {
    void Handle(CTCActivationTrigger* a1, void* a2, void* a3,
                CTCActivationTrigger* a4); // thiscall
};

extern GTarget* g_target; // [0x143ccdc]

// AddTrigger is __fastcall with TWO stack params (ret 8). Modeled as __fastcall:
// self in ecx, an unused edx slot, then the two real stack params p1 and trigger.
// Retail forwards Handle(p1, &self->sub18, self->field20, trigger): the first
// push is trigger (a4), the second load (post-push, esp-shifted) reaches p1 (a1).
void __fastcall AddTrigger(CTCActivationReceptorBase* self, int /*edx*/,
                           void* p1, CTCActivationTrigger* trigger)
{
    g_target->Handle((CTCActivationTrigger*)p1, (void*)&self->sub18,
                     self->field20, trigger);
}