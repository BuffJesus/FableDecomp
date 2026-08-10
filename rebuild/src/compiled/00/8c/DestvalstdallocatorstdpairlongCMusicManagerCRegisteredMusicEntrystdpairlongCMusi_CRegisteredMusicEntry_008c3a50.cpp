struct Sub { int x; };
struct Self {
    char pad[0x114];
    Sub m114;
};
extern "C" void __fastcall SubClean(Sub* s);
extern "C" void __fastcall OtherClean(Self* s);
extern "C" void __fastcall DestVal_114_A(Self* self, void* /*edx*/)
{
    SubClean(&self->m114);
    OtherClean(self);
}