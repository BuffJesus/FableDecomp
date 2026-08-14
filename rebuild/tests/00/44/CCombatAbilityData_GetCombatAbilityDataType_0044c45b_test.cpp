#include <cstdio>

enum ECombatAbilityDataType
{
    ECADT_Base = 0
};

struct CCombatAbilityData
{
    void* vtbl;
};

ECombatAbilityDataType __fastcall CCombatAbilityData_GetCombatAbilityDataType(CCombatAbilityData* self)
{
    return ECADT_Base;
}

int main()
{
    CCombatAbilityData obj;
    obj.vtbl = 0;
    ECombatAbilityDataType t = CCombatAbilityData_GetCombatAbilityDataType(&obj);
    if (t == 0)
        printf("BEHAVIOUR_PASS\n");
    else
        printf("BEHAVIOUR_FAIL\n");
    return 0;
}