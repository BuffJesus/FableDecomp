struct CTCMapwho;

void __fastcall CTCMapwho_Method(CTCMapwho* self);   // 0xaecb80, thiscall
void CTCMapwho_FreeFn(CTCMapwho* p);                 // 0xbfe9bc, cdecl

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_Method(self);
        CTCMapwho_FreeFn(self);
    }
}