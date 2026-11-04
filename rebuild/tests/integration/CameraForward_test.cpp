#include "fable_camera.h"
#ifndef FABLE_CAMERA_FORWARD_ORACLE_HEADER
#define FABLE_CAMERA_FORWARD_ORACLE_HEADER "CameraForward_oracle.h"
#endif
#include FABLE_CAMERA_FORWARD_ORACLE_HEADER
#include <stdio.h>
#include <string.h>
#include <float.h>
static bool Same(unsigned long actual,unsigned long expected) {
    if((expected&0x7f800000UL)==0x7f800000UL && (expected&0x007fffffUL)) return (actual&0x7f800000UL)==0x7f800000UL && (actual&0x007fffffUL)!=0;
    return actual==expected;
}
int main() {
    unsigned oldControl=_control87(0,0);_control87(_PC_53 | _RC_NEAR | _MCW_EM,_MCW_PC | _MCW_RC | _MCW_EM);unsigned scenarios=0;
    for(unsigned i=0;i<sizeof(forwardCases)/sizeof(forwardCases[0]);++i) {
        CCamera c;memset(&c,0xa9,sizeof(c));memcpy(&c.WorldPosition,&forwardCases[i][0],12);memcpy(&c.RHSet.Forward,&forwardCases[i][3],12);c.HeightLocked=forwardCases[i][6]!=0;
        unsigned char expected[sizeof(c)];memcpy(expected,&c,sizeof(c));memcpy(expected,&forwardCases[i][8],12);float distance;memcpy(&distance,&forwardCases[i][7],4);c.MoveForward(distance);++scenarios;
        unsigned long actual[3];memcpy(actual,&c.WorldPosition,12);
        if(!Same(actual[0],forwardCases[i][8]) || !Same(actual[1],forwardCases[i][9]) || !Same(actual[2],forwardCases[i][10]) || memcmp((unsigned char*)&c+12,expected+12,sizeof(c)-12)) {printf("FORWARD_FAIL case=%u actual=%08lx,%08lx,%08lx expected=%08lx,%08lx,%08lx\n",i,actual[0],actual[1],actual[2],forwardCases[i][8],forwardCases[i][9],forwardCases[i][10]);return 1;}
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_FORWARD_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
