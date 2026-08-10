struct CTCMapwho;

extern "C" void __fastcall Mapwho_Member(CTCMapwho* self);
extern "C" int  __cdecl    Mapwho_Helper(CTCMapwho* self);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self) {
        Mapwho_Member(self);
        Mapwho_Helper(self);
    }
}