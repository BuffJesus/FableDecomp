#include "rebuild_abi.h"
// CEngine::SetEnvironmentalEffectsAlwaysUpdate @ 0x00b23b90
// mov al,[esp+4]; mov ecx,ds:g_obj; mov [ecx+0x650],al; ret 4
// Store the bool flag into a field of a globally-held object.
struct CEnvHost { char pad[0x650]; unsigned char flag; };
extern CEnvHost *g_envHost_b23b90;
struct CEngine { void SetEnvironmentalEffectsAlwaysUpdate(bool on); };
void CEngine::SetEnvironmentalEffectsAlwaysUpdate(bool on)
{
    g_envHost_b23b90->flag = (unsigned char)on;
}