#pragma optimize("s",on)
struct GBlob { char pad[0xa0]; float thresh; };
struct GHolder { char pad[0xdc]; GBlob* blob; };
struct CTCHeroStats { char pad[0x19c]; float drunk; };
extern "C" GHolder* GetGlobal(void);
bool __fastcall CTCHeroStats_IsDrunk(const CTCHeroStats* self)
{
    GHolder* h = GetGlobal();
    return self->drunk >= h->blob->thresh;
}