struct Hero {
    char pad[0x91];
    unsigned char flags;
};

struct World { int dummy; };
struct GSI { char pad[0x14]; World* world; };

extern "C" World* __fastcall GSI_GetWorld(World* w);
extern "C" Hero*  __fastcall World_GetHero(World* w);
extern "C" bool   __fastcall Hero_CheckChild(Hero* h);

bool __fastcall CGameScriptInterface_IsHeroChild(GSI* self)
{
    Hero* h = World_GetHero((World*)GSI_GetWorld(self->world));
    if (h != 0 && (h->flags & 1) == 0)
        return Hero_CheckChild(h);
    return false;
}