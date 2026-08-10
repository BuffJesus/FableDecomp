#pragma optimize("t",on)

struct CTCMapwho { int dummy; };

extern "C" void __fastcall Mapwho_Dtor(CTCMapwho* self);
extern "C" void __cdecl Mapwho_Free(void* p);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        Mapwho_Dtor(self);
        Mapwho_Free(self);
    }
}