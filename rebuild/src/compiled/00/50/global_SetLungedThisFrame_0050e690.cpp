extern int __cdecl NBalverineCombat_GetRandomValue();

int __fastcall NBalverineCombat_SetLungedThisFrame(int n)
{
    return NBalverineCombat_GetRandomValue() % n;
}