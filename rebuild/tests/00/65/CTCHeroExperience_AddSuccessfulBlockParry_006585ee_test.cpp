#include <cstdio>
struct CTCHeroExperience { char pad[0xd8]; int value; };
static int g_prep = 0;
extern "C" void __cdecl helper_prep(void) { g_prep = 1; }
extern "C" int __cdecl helper_get(void) { return 0x1234; }
void __fastcall CTCHeroExperience_AddSuccessfulBlockParry(CTCHeroExperience* self)
{
    helper_prep();
    self->value = helper_get();
}
int main() {
    CTCHeroExperience obj;
    obj.value = 0;
    CTCHeroExperience_AddSuccessfulBlockParry(&obj);
    if (g_prep == 1 && obj.value == 0x1234) {
        std::printf("CTCHeroExperience_006585ee_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}