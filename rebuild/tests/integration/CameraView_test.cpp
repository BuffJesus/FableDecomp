#include "fable_camera.h"
#ifndef FABLE_CAMERA_VIEW_ORACLE_HEADER
#define FABLE_CAMERA_VIEW_ORACLE_HEADER "CameraView_oracle.h"
#endif
#include FABLE_CAMERA_VIEW_ORACLE_HEADER
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
    for(unsigned pattern=0;pattern<2;++pattern) {
        for(unsigned i=0;i<sizeof(normalCases)/sizeof(normalCases[0]);++i) {
            unsigned char storage[44],before[44];memset(storage,pattern*213,sizeof(storage));memcpy(storage+16,&normalCases[i][0],12);memcpy(before,storage,sizeof(storage));C3DVector* vector=reinterpret_cast<C3DVector*>(storage+16);if(&vector->AccurateNormalise()!=vector) return 1;++scenarios;
            unsigned long actual[3];memcpy(actual,vector,12);for(unsigned j=0;j<3;++j) if(!Same(actual[j],normalCases[i][3+j])) {printf("NORMAL_FAIL case=%u component=%u actual=%08lx expected=%08lx\n",i,j,actual[j],normalCases[i][3+j]);return 2;}
            if(memcmp(storage,before,16) || memcmp(storage+28,before+28,16)) return 3;
        }
#ifndef FABLE_CATALOG_NORMALIZE_ONLY
        for(unsigned i=0;i<sizeof(viewCases)/sizeof(viewCases[0]);++i) {
            unsigned char storage[sizeof(CCamera)+32],before[sizeof(storage)];memset(storage,pattern*213,sizeof(storage));CCamera* camera=reinterpret_cast<CCamera*>(storage+16);memcpy(&camera->WorldPosition,&viewCases[i][0],36);memcpy(before,storage,sizeof(storage));C3DVector externalUp,externalForward;memcpy(&externalUp,&viewCases[i][9],12);memcpy(&externalForward,&viewCases[i][12],12);C3DVector upBefore=externalUp,forwardBefore=externalForward;const C3DVector* ups[]={&externalUp,&camera->WorldPosition,&camera->RHSet.Up,&camera->RHSet.Forward};const C3DVector* forwards[]={&externalForward,&camera->WorldPosition,&camera->RHSet.Up,&camera->RHSet.Forward};camera->SetView(*forwards[viewCases[i][16]],*ups[viewCases[i][15]]);++scenarios;
            unsigned long actual[6];memcpy(actual,&camera->RHSet,24);for(unsigned j=0;j<6;++j) if(!Same(actual[j],viewCases[i][17+j])) {printf("VIEW_FAIL case=%u component=%u actual=%08lx expected=%08lx\n",i,j,actual[j],viewCases[i][17+j]);return 4;}
            if(memcmp(storage,before,28) || memcmp(storage+52,before+52,sizeof(storage)-52) || memcmp(&externalUp,&upBefore,12) || memcmp(&externalForward,&forwardBefore,12)) return 5;
        }
#endif
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_VIEW_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
