#include "fable_camera.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static const unsigned long inputs[]={0x00000000,0x80000000,0x00000001,0x80000001,0x007fffff,0x807fffff,0x00800000,0x80800000,0x3f800000,0xbf800000,0x3f000000,0xbf000000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,0xffc12345,0x7f812345};
int main() {
    unsigned oldControl=_control87(0,0);_control87(_PC_53 | _RC_NEAR | _MCW_EM,_MCW_PC | _MCW_RC | _MCW_EM);unsigned scenarios=0;
    for(unsigned h=0;h<19;++h) for(unsigned v=0;v<19;++v) for(unsigned pattern=0;pattern<4;++pattern) {
        unsigned char storage[sizeof(CCamera)+32],expected[sizeof(storage)];memset(storage,pattern*71,sizeof(storage));memcpy(expected,storage,sizeof(storage));CCamera* camera=reinterpret_cast<CCamera*>(storage+16);
        unsigned long vertical=inputs[v];bool nan=(vertical&0x7f800000UL)==0x7f800000UL && (vertical&0x7fffffUL)!=0;unsigned long flags=(vertical&0x80000000UL)==0 && (vertical&0x7fffffffUL)!=0 && !nan?3:1;memcpy(expected+16+0x28,&flags,4);memcpy(expected+16+0x2c,&inputs[h],4);memcpy(expected+16+0x30,&inputs[v],4);
        float horizontalAngle,verticalAngle;memcpy(&horizontalAngle,&inputs[h],4);memcpy(&verticalAngle,&inputs[v],4);camera->SetFOV(horizontalAngle,verticalAngle);++scenarios;
        if(memcmp(storage,expected,sizeof(storage))) {printf("FOV_FAIL h=%u v=%u pattern=%u\n",h,v,pattern);return 1;}
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_FOV_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
