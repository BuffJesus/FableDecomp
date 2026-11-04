#include "fable_camera.h"
#ifndef FABLE_CAMERA_ANGLE_YZ_ORACLE_HEADER
#define FABLE_CAMERA_ANGLE_YZ_ORACLE_HEADER "CameraAngleYZ_oracle.h"
#endif
#include FABLE_CAMERA_ANGLE_YZ_ORACLE_HEADER
#include <stdio.h>
#include <string.h>
#include <float.h>
static bool Same(unsigned long actual,unsigned long expected) {
    if((expected&0x7f800000UL)==0x7f800000UL && (expected&0x007fffffUL)) return (actual&0x7f800000UL)==0x7f800000UL && (actual&0x007fffffUL)!=0;
    if((expected&0x7fffffffUL)==0) return actual==expected;
    if((actual&0x80000000UL)!=(expected&0x80000000UL)) return false;
    unsigned long distance=actual>expected?actual-expected:expected-actual;return distance<=2;
}
int main() {
    unsigned oldControl=_control87(0,0);_control87(_PC_53 | _RC_NEAR | _MCW_EM,_MCW_PC | _MCW_RC | _MCW_EM);unsigned scenarios=0;
    for(unsigned i=0;i<sizeof(angleYZCases)/sizeof(angleYZCases[0]);++i) {
        CCamera c;memset(&c,0xa9,sizeof(c));memcpy(&c.RHSet.Forward.X,&angleYZCases[i][0],4);memcpy(&c.RHSet.Forward.Y,&angleYZCases[i][1],4);memcpy(&c.RHSet.Forward.Z,&angleYZCases[i][2],4);
        unsigned char before[sizeof(c)];memcpy(before,&c,sizeof(c));float angle=c.GetAngleYZ();unsigned long bits;memcpy(&bits,&angle,4);++scenarios;
        if(!Same(bits,angleYZCases[i][3]) || memcmp(before,&c,sizeof(c))) {printf("ANGLE_YZ_FAIL case=%u actual=%08lx expected=%08lx\n",i,bits,angleYZCases[i][3]);return 1;}
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_ANGLE_YZ_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
