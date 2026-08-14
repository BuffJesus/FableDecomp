// callee at 0x6b9e00 is a __fastcall member on the trigger: ecx=trigger, stack arg = void*
struct CTCActivationTrigger {
    void Method(void* p);   // real member -> this in ecx, p on stack, no edx
};

struct CTCActivationReceptorBase {
    char pad24[0x24];   // +0x00
    void* field24;      // +0x24
    void* p28;          // +0x28
};

bool __fastcall AddTrigger(CTCActivationReceptorBase* self, void* edx, CTCActivationTrigger* trig)
{
    trig->Method(&self->field24);
    return self->p28 != 0;
}