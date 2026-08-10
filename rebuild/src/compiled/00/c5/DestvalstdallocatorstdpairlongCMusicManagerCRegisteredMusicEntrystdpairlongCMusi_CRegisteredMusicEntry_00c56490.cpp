struct Sub;
struct T;
void __fastcall Sub_Cleanup(Sub* self);
void __fastcall T_Base(T* self);
struct Sub { int q; };
struct T { char pad[0x110]; Sub sub; };
void __fastcall DestVal_110(T* self, void* /*edx*/)
{
    Sub_Cleanup(&self->sub);
    T_Base(self);
}