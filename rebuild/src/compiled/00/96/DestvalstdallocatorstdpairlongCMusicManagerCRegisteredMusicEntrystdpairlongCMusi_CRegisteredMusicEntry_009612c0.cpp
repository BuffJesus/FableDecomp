struct Sub { char pad[4]; };
struct Owner { char pad[0xa8]; Sub sub; };

extern "C" void __fastcall Sub_Init(Sub* s);
extern "C" void __fastcall Tail_Cleanup(Owner* o);

extern "C" void __fastcall DestVal_A8_J(Owner* self)
{
    Sub_Init(&self->sub);
    Tail_Cleanup(self);
}