struct Sub { void* p; };
struct Obj {
  char pad[0x114];
  Sub sub;
};
extern "C" void __fastcall sub_helper(Sub* s);
extern "C" void __fastcall base_dtor(Obj* o);
extern "C" void __fastcall DestVal_114_E(Obj* self, void* /*edx*/)
{
  sub_helper(&self->sub);
  base_dtor(self);
}