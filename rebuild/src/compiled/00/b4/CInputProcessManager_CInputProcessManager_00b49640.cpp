struct CInputProcessManager { void* vtbl; };

extern "C" void __fastcall inner_dtor(CInputProcessManager* self);
extern "C" void __fastcall base_dtor(CInputProcessManager* self);

void __fastcall DtorIPM_F(CInputProcessManager* self)
{
    self->vtbl = (void*)0x12a20fc;
    inner_dtor(self);
    base_dtor(self);
}