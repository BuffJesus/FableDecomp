// Retail 00a2e710: mov ecx,[ecx+4]; jmp 0xa75430
// VOID tail-jmp forwarder: forwards *(void**)(self+4) to an extern __fastcall target.

struct SelfT {
    void* pad0;   // +0
    void* member; // +4
};

extern void __fastcall _target_a75430(void* self);

void __fastcall _Cons_val_pair_EHeroMorphType_CEntry(SelfT* self)
{
    _target_a75430(self->member);
}