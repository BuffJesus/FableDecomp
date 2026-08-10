struct Sub { char pad[4]; };
struct Obj {
    char pad[0xa8];
    Sub sub;
};
extern "C" void __fastcall SubDtor_a01c10(Sub* s);
extern "C" void __fastcall BaseDtor_693ef0(Obj* o);
extern "C" void __fastcall DestVal_A8_F(Obj* self)
{
    SubDtor_a01c10(&self->sub);
    BaseDtor_693ef0(self);
}