#include <stdlib.h>

struct CTCMapwho { int dummy; };

extern "C" void __fastcall Mapwho_PreDie(CTCMapwho* self);
extern "C" void __cdecl Mapwho_Free(void* p);

void __fastcall OnDie_Mapwho(CTCMapwho* self)
{
    if (self)
    {
        Mapwho_PreDie(self);
        Mapwho_Free(self);
    }
}