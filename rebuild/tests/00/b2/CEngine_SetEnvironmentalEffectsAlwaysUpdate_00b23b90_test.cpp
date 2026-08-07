#include "rebuild_abi.h"
#include <cstdio>
struct CEnvHost { char pad[0x650]; unsigned char flag; };
CEnvHost g_storage_b23b90;
CEnvHost *g_envHost_b23b90 = &g_storage_b23b90;
struct CEngine { void SetEnvironmentalEffectsAlwaysUpdate(bool on); };
void CEngine::SetEnvironmentalEffectsAlwaysUpdate(bool on){ g_envHost_b23b90->flag=(unsigned char)on; }
int main(){
    CEngine e; e.SetEnvironmentalEffectsAlwaysUpdate(true);
    if(g_storage_b23b90.flag!=1){std::printf("BAD\n");return 1;}
    e.SetEnvironmentalEffectsAlwaysUpdate(false);
    if(g_storage_b23b90.flag!=0){std::printf("BAD0\n");return 1;}
    std::printf("SETENV_OK\n"); return 0;
}