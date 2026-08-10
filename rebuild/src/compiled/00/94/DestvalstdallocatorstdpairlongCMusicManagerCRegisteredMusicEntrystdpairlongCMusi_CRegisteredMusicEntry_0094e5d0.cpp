struct Sub { char pad[8]; };
struct Base { char pad[8]; };
extern "C" void __fastcall SubDtor(Sub* s);
extern "C" void __fastcall BaseDtor(Base* b);
struct Obj {
  char pad[0x114];
  Sub sub;
};
extern "C" void __fastcall DestVal_114_C(Obj* self, void* /*edx*/) {
  SubDtor(&self->sub);
  BaseDtor((Base*)self);
}