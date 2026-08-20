extern "C" void __fastcall MemberDtor_686df0(void* self);
extern "C" void __fastcall BaseDtor_686830(void* self);

struct CInputProcessManager {
    void** vtbl;
};

extern const void* const g_vtbl_CInputProcessManager[];

extern "C" void __fastcall DtorInputProcessManager(CInputProcessManager* self, int /*edx*/)
{
    self->vtbl = (void**)&g_vtbl_CInputProcessManager;
    MemberDtor_686df0(self);
    BaseDtor_686830(self);
}