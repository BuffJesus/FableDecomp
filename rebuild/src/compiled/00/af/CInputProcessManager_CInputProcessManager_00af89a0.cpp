struct CInputProcessManager {
    void* vptr;
};

extern void __fastcall Helper_af8780(CInputProcessManager* self);
extern void __fastcall Tail_f36c90(CInputProcessManager* self);

void __fastcall DtorIPM_A(CInputProcessManager* self)
{
    self->vptr = (void*)0x0129F638;
    Helper_af8780(self);
    Tail_f36c90(self);
}