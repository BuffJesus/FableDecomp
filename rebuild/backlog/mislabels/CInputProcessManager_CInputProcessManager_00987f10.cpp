struct CInputProcessManager {
  void* vtbl;
};
extern "C" void __fastcall sub_A5C600(CInputProcessManager* self);
extern "C" void __fastcall base_dtor_99A300(CInputProcessManager* self);
void __fastcall DtorInputProcessManager2(CInputProcessManager* self)
{
  self->vtbl = (void*)0x129a064;
  sub_A5C600(self);
  base_dtor_99A300(self);
}