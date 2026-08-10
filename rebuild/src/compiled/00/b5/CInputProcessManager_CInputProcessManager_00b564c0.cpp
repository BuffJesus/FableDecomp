extern "C" void __fastcall sub_a63200(void* self);
extern "C" void __fastcall base_dtor_99a300(void* self);

struct CInputProcessManager {
    void* vptr;
};

extern "C" void __fastcall DtorIPM_G(CInputProcessManager* self)
{
    self->vptr = (void*)0x129ba20;
    sub_a63200(self);
    base_dtor_99a300(self);
}