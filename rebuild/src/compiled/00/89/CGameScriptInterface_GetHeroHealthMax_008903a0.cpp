struct CThing;
struct CHero;

extern "C" CThing* __fastcall GetThing(void* p);
extern "C" CHero*  __fastcall ResolveHero(void* p);
extern float g_defaultHealthMax;

struct CGameScriptInterface {
    char pad[0x14];
    void* member14;
};

float __fastcall CGameScriptInterface_GetHeroHealthMax(CGameScriptInterface* self)
{
    CThing* t = GetThing(self->member14);
    CHero* h = ResolveHero(t);
    if (h != 0) {
        if ((*(unsigned char*)((char*)h + 0x91) & 1) == 0) {
            return *(float*)((char*)h + 0xb0);
        }
    }
    return g_defaultHealthMax;
}