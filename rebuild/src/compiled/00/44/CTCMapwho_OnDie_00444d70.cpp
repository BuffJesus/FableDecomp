struct CTCMapwho;
extern "C" void __fastcall CTCMapwho_Cleanup(CTCMapwho* self);
extern "C" void FreeMapwho(CTCMapwho* self);

void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_Cleanup(self);
        FreeMapwho(self);
    }
}