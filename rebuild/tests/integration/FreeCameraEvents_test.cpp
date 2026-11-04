#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
#include <stdio.h>
#include <string.h>
static CPlayerManager* expectedManager;static CPlayer* expectedPlayer;static long expectedNumber;static unsigned calls,errors;static bool enable;
CPlayer& CPlayerManager::GetPlayer(long number){if(this!=expectedManager || number!=expectedNumber || calls)++errors;++calls;return *expectedPlayer;}
void CPlayer::AddMode(EPlayerMode mode,bool force){if(this!=expectedPlayer || !enable || mode!=PLAYER_MODE_CONTROL_CAMERA || force || calls!=1)++errors;++calls;}
void CPlayer::RemoveMode(EPlayerMode mode){if(this!=expectedPlayer || enable || mode!=PLAYER_MODE_CONTROL_CAMERA || calls!=1)++errors;++calls;}
int main(){unsigned scenarios=0;const unsigned lengths[4]={1,2,8,32};
    for(unsigned pattern=0;pattern<4;++pattern){unsigned long ws[104],ps[160];memset(ws,pattern*71,sizeof(ws));memset(ps,pattern*71,sizeof(ps));CWorld* world=reinterpret_cast<CWorld*>(ws+2);expectedPlayer=reinterpret_cast<CPlayer*>(ps+2);expectedManager=reinterpret_cast<CPlayerManager*>(0x14500000+pattern*0x1000);world->PlayerManager=expectedManager;
        for(unsigned value=0;value<256;++value)for(unsigned number=0;number<256;++number)for(unsigned l=0;l<4;++l)for(unsigned which=0;which<2;++which){CGameEvent event(0x7fffffff);memset(event.Data,value,32);event.Player=(signed char)number;event.EndPos=lengths[l];CGameEvent eventBefore;memcpy(&eventBefore,&event,sizeof(event));unsigned long wb[104],pb[160];memcpy(wb,ws,sizeof(ws));memcpy(pb,ps,sizeof(ps));enable=value==1;expectedNumber=(signed char)number;calls=errors=0;
            if(which==0)world->EASetFreeCameraMode(&event);else{world->EAUseFreeCamera(&event);reinterpret_cast<unsigned char*>(pb+2)[0x220]=enable?1:0;}
            if(errors || calls!=(which==0?2:1) || memcmp(wb,ws,sizeof(ws)) || memcmp(pb,ps,sizeof(ps)) || memcmp(&eventBefore,&event,sizeof(event)))return 1;++scenarios;}
    }
    printf("FREE_CAMERA_EVENTS_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
