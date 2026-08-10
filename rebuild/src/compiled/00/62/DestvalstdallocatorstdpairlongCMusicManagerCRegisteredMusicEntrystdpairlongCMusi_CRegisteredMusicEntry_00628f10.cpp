struct Sub { int q; };
struct M { char pad[0xa8]; Sub sub; };
extern "C" void __fastcall helper_sub(Sub* s);
extern "C" void __fastcall final_music(M* self);
void __fastcall DestVal_Music_A(M* self, void* /*edx*/)
{
    helper_sub(&self->sub);
    final_music(self);
}