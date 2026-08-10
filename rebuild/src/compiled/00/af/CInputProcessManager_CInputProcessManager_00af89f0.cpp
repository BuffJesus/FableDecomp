struct CInputProcessManager {
    void* vptr;
};

extern void __fastcall SubCleanup(CInputProcessManager* self);  // 0xaf8860
extern void __fastcall BaseDtor(CInputProcessManager* self);    // 0xf36c90

void __fastcall DtorIPM_B(CInputProcessManager* self)
{
    self->vptr = (void*)0x0129f644;
    SubCleanup(self);
    BaseDtor(self);
}