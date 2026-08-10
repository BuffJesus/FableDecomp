struct CTCMapwho;
extern void __fastcall CTCMapwho_helper(CTCMapwho* self);
extern void __cdecl external_cdecl(CTCMapwho* self);
void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_helper(self);
        external_cdecl(self);
    }
}