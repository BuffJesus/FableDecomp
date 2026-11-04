#include "fable_camera.h"
#ifndef FABLE_CAMERA_POINT_AT_ORACLE_HEADER
#define FABLE_CAMERA_POINT_AT_ORACLE_HEADER "CameraPointAt_oracle.h"
#endif
#include FABLE_CAMERA_POINT_AT_ORACLE_HEADER
#include <stdio.h>
#include <string.h>
#include <float.h>
static bool Same(unsigned long actual,unsigned long expected) {
    if((expected&0x7f800000UL)==0x7f800000UL && (expected&0x007fffffUL)) return (actual&0x7f800000UL)==0x7f800000UL && (actual&0x007fffffUL)!=0;
    if((expected&0x7fffffffUL)==0 || (expected&0x7f800000UL)==0x7f800000UL) return actual==expected;
    if((actual>>31)!=(expected>>31)) return false;
    return actual>expected ? actual-expected<=2 : expected-actual<=2;
}
int main() {
    unsigned oldControl=_control87(0,0);_control87(_PC_53 | _RC_NEAR | _MCW_EM,_MCW_PC | _MCW_RC | _MCW_EM);unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) for(unsigned i=0;i<sizeof(pointCases)/sizeof(pointCases[0]);++i) {
        unsigned char storage[sizeof(CCamera)+32],before[sizeof(storage)];memset(storage,pattern*71,sizeof(storage));CCamera* camera=reinterpret_cast<CCamera*>(storage+16);memcpy(&camera->WorldPosition,&pointCases[i][0],36);camera->ViewVecZLocked=pointCases[i][12]!=0;memcpy(before,storage,sizeof(storage));C3DVector externalTarget;memcpy(&externalTarget,&pointCases[i][9],12);C3DVector targetBefore=externalTarget;const C3DVector* targets[]={&externalTarget,&camera->WorldPosition,&camera->RHSet.Up,&camera->RHSet.Forward};camera->PointAt(*targets[pointCases[i][13]]);++scenarios;
        unsigned long actual[6];memcpy(actual,&camera->RHSet,24);for(unsigned j=0;j<6;++j) if(!Same(actual[j],pointCases[i][14+j])) {printf("POINT_AT_FAIL case=%u component=%u actual=%08lx expected=%08lx\n",i,j,actual[j],pointCases[i][14+j]);return 1;}
        if(memcmp(storage,before,28) || memcmp(storage+52,before+52,sizeof(storage)-52) || memcmp(&externalTarget,&targetBefore,12)) return 2;
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_POINT_AT_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
