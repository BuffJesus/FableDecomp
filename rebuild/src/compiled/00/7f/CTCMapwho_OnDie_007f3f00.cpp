struct CTCMapwho;
extern "C" void __fastcall Mapwho_Cleanup(CTCMapwho* self);
extern "C" void __cdecl Mapwho_Release(CTCMapwho* self);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        Mapwho_Cleanup(self);
        Mapwho_Release(self);
    }
}