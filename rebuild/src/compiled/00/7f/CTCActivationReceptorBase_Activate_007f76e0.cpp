struct CTCActivationReceptorBase {
    char pad0[0x0c];
    unsigned char f0c;   // +0x0c
    char pad1[0x20 - 0x0d];
    unsigned char f20;   // +0x20
    char pad2[0x28 - 0x21];
    void* f28;           // +0x28
};

extern void* __fastcall helper_49d870(CTCActivationReceptorBase* self);

void __fastcall Activate(CTCActivationReceptorBase* self)
{
    self->f0c = 1;
    if (self->f20)
        self->f28 = helper_49d870(self);
}