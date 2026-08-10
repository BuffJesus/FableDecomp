// CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance
// retail: fld dword ptr [0x122f59c]; ret  -> return a float global
struct CAIStateGroup_EnemyCreatureBase;

extern float g_CombatMinDistance;

float __fastcall GetCombatMinDistance(CAIStateGroup_EnemyCreatureBase* self)
{
    return g_CombatMinDistance;
}