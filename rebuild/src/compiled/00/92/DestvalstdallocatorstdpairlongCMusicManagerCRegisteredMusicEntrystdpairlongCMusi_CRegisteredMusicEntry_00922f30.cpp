struct Sub { int x; };
struct T { char pad[0xa8]; Sub sub; };

extern "C" void __fastcall SubCall(Sub* s);
extern "C" void __fastcall FinalCall(T* t);

void __fastcall DestVal_A8_C(T* self)
{
    SubCall(&self->sub);
    FinalCall(self);
}