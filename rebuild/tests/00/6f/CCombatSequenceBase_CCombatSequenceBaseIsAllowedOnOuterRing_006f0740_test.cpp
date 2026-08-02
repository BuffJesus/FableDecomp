#include <cstdio>

struct CCombatSequenceBase_vtbl
{
    void* slots[16];
};

struct CCombatSequenceBase
{
    CCombatSequenceBase_vtbl* vtbl;
};

typedef unsigned int (__fastcall *IsAllowedOnOuterRing_fn)(CCombatSequenceBase* self, void* edx);

unsigned int __fastcall CCombatSequenceBase_IsAllowedOnOuterRing(CCombatSequenceBase* self, void* edx)
{
    return (((IsAllowedOnOuterRing_fn)(*(void***)self)[0xF])(self, edx) >> 7) & 1;
}

static int g_calls = 0;

unsigned int __fastcall FakeVirtual(CCombatSequenceBase* self, void* edx)
{
    g_calls = g_calls + 1;
    return 0x80;
}

int main()
{
    CCombatSequenceBase_vtbl vtbl;
    for (int i = 0; i < 16; i = i + 1)
    {
        vtbl.slots[i] = 0;
    }
    vtbl.slots[0xF] = (void*)FakeVirtual;

    CCombatSequenceBase obj;
    obj.vtbl = &vtbl;

    unsigned int result = CCombatSequenceBase_IsAllowedOnOuterRing(&obj, 0);

    if (result == 1 && g_calls == 1)
    {
        printf("CCombatSequenceBase_IsAllowedOnOuterRing_TEST_PASS\n");
    }
    else
    {
        printf("CCombatSequenceBase_IsAllowedOnOuterRing_TEST_FAIL result=%u calls=%d\n", result, g_calls);
    }

    return 0;
}