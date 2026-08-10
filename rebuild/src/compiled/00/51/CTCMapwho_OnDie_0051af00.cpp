struct CTCMapwho;
extern "C" void __fastcall CTCMapwho_dtor(CTCMapwho* self);
extern "C" void __cdecl fable_opdel(void* p);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        CTCMapwho_dtor(self);
        fable_opdel(self);
    }
}