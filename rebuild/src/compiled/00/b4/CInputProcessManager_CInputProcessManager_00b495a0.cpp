struct CInputProcessManager {
    void* vtbl;
};

extern int __fastcall sub_b48c70(CInputProcessManager* self);
extern void __fastcall base_dtor_f36c90(CInputProcessManager* self);

void __fastcall DtorIPM_D(CInputProcessManager* self)
{
    self->vtbl = (void*)0x012a20e4;
    sub_b48c70(self);
    base_dtor_f36c90(self);
}