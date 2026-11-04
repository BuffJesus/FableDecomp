#include "fable_camera.h"
#ifndef FABLE_CAMERA_SIDEWAYS_ORACLE_HEADER
#define FABLE_CAMERA_SIDEWAYS_ORACLE_HEADER "CameraSideways_oracle.h"
#endif
#include FABLE_CAMERA_SIDEWAYS_ORACLE_HEADER
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
    for(unsigned i=0;i<sizeof(sidewaysCases)/sizeof(sidewaysCases[0]);++i) {
        CCamera c;memset(&c,0xa9,sizeof(c));memcpy(&c.WorldPosition,&sidewaysCases[i][0],12);memcpy(&c.RHSet.Up,&sidewaysCases[i][3],12);memcpy(&c.RHSet.Forward,&sidewaysCases[i][6],12);c.HeightLocked=sidewaysCases[i][9]!=0;
        unsigned char before[sizeof(c)];memcpy(before,&c,sizeof(c));float distance;memcpy(&distance,&sidewaysCases[i][10],4);c.MoveSideways(distance);++scenarios;
        unsigned long actual[3];memcpy(actual,&c.WorldPosition,12);
        if(!Same(actual[0],sidewaysCases[i][11]) || !Same(actual[1],sidewaysCases[i][12]) || !Same(actual[2],sidewaysCases[i][13]) || memcmp((unsigned char*)&c+12,before+12,sizeof(c)-12)) {printf("SIDEWAYS_FAIL case=%u actual=%08lx,%08lx,%08lx expected=%08lx,%08lx,%08lx\n",i,actual[0],actual[1],actual[2],sidewaysCases[i][11],sidewaysCases[i][12],sidewaysCases[i][13]);return 1;}
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_SIDEWAYS_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
