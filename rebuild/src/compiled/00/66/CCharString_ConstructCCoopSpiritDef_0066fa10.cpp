extern "C" void* __fastcall
CCharString_ConstructCCoopSpiritDef_0066fa10_Construct(
    void* string, void*, const char* text, long length);
extern "C" char CCharString_ConstructCCoopSpiritDef_0066fa10_Name[];

extern "C" __declspec(naked) void* __fastcall
CCharString_ConstructCCoopSpiritDef_0066fa10(
    void* string, void*)
{
    __asm
    {
        push esi
        push -1
        push offset CCharString_ConstructCCoopSpiritDef_0066fa10_Name
        mov esi, ecx
        call CCharString_ConstructCCoopSpiritDef_0066fa10_Construct
        mov eax, esi
        pop esi
        ret
    }
}
