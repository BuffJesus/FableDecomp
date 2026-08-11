#include <cstdio>
#include <cstring>

struct CCharString {
    char* p;
    void ctor(const char* s, int n);
};

void CCharString::ctor(const char* s, int n)
{
    p = (char*)s;
}

struct CCombatSequence_VillagerComboUnblockable {
    void* vtbl;
};

extern "C" CCharString* __fastcall CCombatSequence_VillagerComboUnblockable_GetName(
    CCombatSequence_VillagerComboUnblockable* self, int edx, CCharString* ret);

int main()
{
    CCombatSequence_VillagerComboUnblockable obj;
    obj.vtbl = 0;
    CCharString out;
    out.p = 0;
    CCharString* r = CCombatSequence_VillagerComboUnblockable_GetName(&obj, 0, &out);
    if (r != &out) { printf("FAIL ret\n"); return 1; }
    if (out.p == 0 || strcmp(out.p, "VillagerComboUnblockable") != 0) { printf("FAIL str\n"); return 1; }
    printf("GETNAME_VILLAGER_OK\n");
    return 0;
}