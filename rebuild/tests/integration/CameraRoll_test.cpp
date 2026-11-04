#include "fable_camera.h"
#ifndef FABLE_CAMERA_ROLL_ORACLE_HEADER
#define FABLE_CAMERA_ROLL_ORACLE_HEADER "CameraRoll_oracle.h"
#endif
#include FABLE_CAMERA_ROLL_ORACLE_HEADER
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <errno.h>
static bool Same(unsigned long actual,unsigned long expected) {
    if((expected&0x7f800000UL)==0x7f800000UL && (expected&0x007fffffUL)) return (actual&0x7f800000UL)==0x7f800000UL && (actual&0x007fffffUL)!=0;
    if((expected&0x7fffffffUL)==0) return actual==expected;
    if((actual&0x80000000UL)!=(expected&0x80000000UL)) return false;
    unsigned long distance=actual>expected?actual-expected:expected-actual;return distance<=2;
}
int main() {
    unsigned oldControl=_control87(0,0);_control87(_PC_53 | _RC_NEAR | _MCW_EM,_MCW_PC | _MCW_RC | _MCW_EM);unsigned scenarios=0;
    for(unsigned i=0;i<sizeof(rollCases)/sizeof(rollCases[0]);++i) {
        CCamera c;memset(&c,0xa9,sizeof(c));memcpy(&c.RHSet.Up,&rollCases[i][0],12);memcpy(&c.RHSet.Forward,&rollCases[i][3],12);
        unsigned char before[sizeof(c)];memcpy(before,&c,sizeof(c));errno=0;float angle=c.GetAngleXZ();unsigned long bits;memcpy(&bits,&angle,4);++scenarios;
        if(!Same(bits,rollCases[i][6]) || errno!=(int)rollCases[i][7] || memcmp(before,&c,sizeof(c))) {printf("ROLL_FAIL case=%u actual=%08lx expected=%08lx errno=%d expected_errno=%lu\n",i,bits,rollCases[i][6],errno,rollCases[i][7]);return 1;}
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_ROLL_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
