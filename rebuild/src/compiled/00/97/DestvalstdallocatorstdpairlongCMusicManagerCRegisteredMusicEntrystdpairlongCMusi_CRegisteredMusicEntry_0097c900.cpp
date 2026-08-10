struct Sub { char pad[4]; };
struct Obj { char pad[0x118]; Sub sub; };

extern "C" void __fastcall Sub_Init(Sub* s);
extern "C" void __fastcall Base_Do(Obj* o);

extern "C" void __fastcall DestVal_118_A(Obj* self)
{
    Sub_Init(&self->sub);
    Base_Do(self);
}