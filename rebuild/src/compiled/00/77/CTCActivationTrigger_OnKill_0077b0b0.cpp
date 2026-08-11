struct CTCActivationTrigger;

// helper at 0x7f7750: called with ecx = this->fc, one stack arg = this (__fastcall)
struct Sub {
    void destroy(CTCActivationTrigger* owner);
};

struct CTCActivationTrigger {
    char pad[0xc];
    Sub* fc;   // +0xc
};

void __fastcall CTCActivationTrigger_OnKill(CTCActivationTrigger* self)
{
    if (self->fc) {
        self->fc->destroy(self);
        self->fc = 0;
    }
}