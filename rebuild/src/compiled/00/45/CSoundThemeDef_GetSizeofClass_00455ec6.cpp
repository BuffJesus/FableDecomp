#pragma optimize("s",on)
struct CSoundThemeDef { char pad[0x64]; };

unsigned long __fastcall CSoundThemeDef_GetSizeofClass(const CSoundThemeDef* self)
{
    return 0x64;
}