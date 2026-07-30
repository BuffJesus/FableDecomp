// GetCombatMinDistance @ 0x0040e250
// Disassembly:
//   fld dword ptr ds:[0x122dedc]
//   ret
// Reads a global float and returns it (float return via ST(0)).
extern float g_CombatMinDistance;

float GetCombatMinDistance()
{
    return g_CombatMinDistance;
}