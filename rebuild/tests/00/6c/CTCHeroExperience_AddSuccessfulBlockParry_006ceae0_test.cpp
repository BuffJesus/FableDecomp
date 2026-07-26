#include <cstdio>
struct CTCHeroExperience { char pad[0xc]; int field_c; };
static int g_second = 0x1234;
void First_Helper() { }
int Second_Helper() { return g_second; }
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    First_Helper();
    self->field_c = Second_Helper();
}
int main()
{
    CTCHeroExperience obj;
    obj.field_c = 0;
    CTCHeroExperience_AddSuccessfulBlockParry(&obj);
    if (obj.field_c == 0x1234) { std::printf("CTCHeroExperience_006ceae0_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}