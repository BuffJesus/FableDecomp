// CPersistContextReadSection::CPersistContextReadSection @ 0x00485CD2.
//
// Binary and text game-state loads converge here before reading the HEADER
// section.  Retail constructs the section's three embedded values at +0x2C,
// +0x48, and +0x70, in that order, then returns the section object.

extern "C" void __fastcall
CPersistContextReadSection_ConstructField2C_00485cd2(void*, void*);
extern "C" void __fastcall
CPersistContextReadSection_ConstructField48_00485cd2(void*, void*);
extern "C" void __fastcall
CPersistContextReadSection_ConstructName_00485cd2(void*, void*);

extern "C" __declspec(naked) void* __fastcall
CPersistContextReadSection_CPersistContextReadSection_00485cd2(
    void*,
    void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        lea ecx, dword ptr [esi + 2Ch]
        call CPersistContextReadSection_ConstructField2C_00485cd2
        lea ecx, dword ptr [esi + 48h]
        call CPersistContextReadSection_ConstructField48_00485cd2
        lea ecx, dword ptr [esi + 70h]
        call CPersistContextReadSection_ConstructName_00485cd2
        mov eax, esi
        pop esi
        ret
    }
}
