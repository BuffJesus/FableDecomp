struct CTCMapwho { int dummy; };

extern "C" void __fastcall Mapwho_PreDie(CTCMapwho* self);
extern "C" void __cdecl retail_free(CTCMapwho* p);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self) {
        Mapwho_PreDie(self);
        retail_free(self);
    }
}