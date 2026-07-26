extern int g_blockParryInt;

struct SubObj { char pad[0x44]; float factor44; };
struct BigObj { char pad[0xdc]; SubObj* sub; };

extern BigObj* GetBigObj();

struct CTCHeroExperience {
    char pad[0x64];
    int successfulBlockParry;
};

void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    BigObj* p = GetBigObj();
    self->successfulBlockParry = (int)((float)g_blockParryInt * p->sub->factor44);
}