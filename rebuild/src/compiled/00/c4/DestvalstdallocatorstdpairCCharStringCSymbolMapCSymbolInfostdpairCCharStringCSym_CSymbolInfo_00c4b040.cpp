// Reconstruction of 00c4b040 _Dest_val_pair_CCharString_CSymbolInfo
// push esi; mov esi,ecx; call A(esi); mov ecx,esi; call B(esi); mov ecx,esi; pop esi; jmp A
// = A(this); B(this); return A(this);  (last is tail -> jmp)

extern void __fastcall helperA(void* self);   // 0xc4b010
extern void __fastcall helperB(void* self);   // 0xc4a3b0

void __fastcall _Dest_val_pair_CCharString_CSymbolInfo(void* self, void* /*edx*/)
{
    helperA(self);
    helperB(self);
    helperA(self);
}