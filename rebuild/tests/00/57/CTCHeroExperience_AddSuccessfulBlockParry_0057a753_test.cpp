#include <cstdio>

int g_blockParryInt = 10;

struct SubObj { char pad[0x44]; float factor44; };
struct BigObj { char pad[0xdc]; SubObj* sub; };

static SubObj g_sub;
static BigObj g_big;

BigObj* GetBigObj() { g_big.sub = &g_sub; return &g_big; }

struct CTCHeroExperience {
    char pad[0x64];
    int successfulBlockParry;
};

void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    BigObj* p = GetBigObj();
    self->successfulBlockParry = (int)((float)g_blockParryInt * p->sub->factor44);
}

int main()
{
    g_sub.factor44 = 2.5f;
    CTCHeroExperience h;
    h.successfulBlockParry = 0;
    CTCHeroExperience_AddSuccessfulBlockParry(&h);
    if (h.successfulBlockParry == 25) {
        std::printf("CTCHeroExperience_0057a753_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %d\n", h.successfulBlockParry);
    return 1;
}