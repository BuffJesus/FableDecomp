// CTCElectrocutedEffect::OnInitialActivate @ 006d8770
struct CTCElectrocutedEffect {
    char pad[0x30];
    int f30; // +0x30
};

// helper called with arg 0; no stack cleanup at call site => stdcall
extern void __stdcall helper_6d7f20(int a);
// base tail method (this)
extern void __fastcall base_6d7e70(CTCElectrocutedEffect* self);

void __fastcall CTCElectrocutedEffect_OnInitialActivate(CTCElectrocutedEffect* self)
{
    self->f30 = 0;
    helper_6d7f20(0);
    base_6d7e70(self);
}