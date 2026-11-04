#include "fable_camera.h"
#include <stdio.h>
#include <string.h>
static CCamera* expectedCamera;static unsigned calls,errors;static unsigned long expectedUp[3],expectedForward[3];
static const unsigned long upMark[]={0x11223344,0x22334455,0x33445566},forwardMark[]={0x44556677,0x55667788,0x66778899};
C3DVector& C3DVector::AccurateNormalise() {
    if(calls==0) {if(this!=&expectedCamera->RHSet.Up || memcmp(this,expectedUp,12)) ++errors;memcpy(this,upMark,12);}
    else {if(calls!=1 || this!=&expectedCamera->RHSet.Forward || memcmp(this,expectedForward,12)) ++errors;memcpy(this,forwardMark,12);}
    ++calls;return *this;
}
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) for(unsigned up=0;up<4;++up) for(unsigned forward=0;forward<4;++forward) {
        unsigned char storage[sizeof(CCamera)+32],expected[sizeof(storage)];memset(storage,pattern*71,sizeof(storage));expectedCamera=reinterpret_cast<CCamera*>(storage+16);const unsigned long initial[9]={0x3f000000,0xbf800000,0x41200000,0x7f812345,0x80000000,1,0x3f800000,0x40000000,0x40400000};memcpy(&expectedCamera->WorldPosition,initial,36);memcpy(expected,storage,sizeof(storage));C3DVector externalUp(4,5,6),externalForward(7,8,9);C3DVector upBefore=externalUp,forwardBefore=externalForward;
        const C3DVector* actualUps[]={&externalUp,&expectedCamera->WorldPosition,&expectedCamera->RHSet.Up,&expectedCamera->RHSet.Forward};const C3DVector* actualForwards[]={&externalForward,&expectedCamera->WorldPosition,&expectedCamera->RHSet.Up,&expectedCamera->RHSet.Forward};const unsigned char* expectedUps[]={reinterpret_cast<const unsigned char*>(&externalUp),expected+16,expected+28,expected+40};const unsigned char* expectedForwards[]={reinterpret_cast<const unsigned char*>(&externalForward),expected+16,expected+28,expected+40};
        memcpy(expectedUp,expectedUps[up],12);memcpy(expected+28,expectedUp,12);memcpy(expectedForward,expectedForwards[forward],12);memcpy(expected+40,expectedForward,12);memcpy(expected+28,upMark,12);memcpy(expected+40,forwardMark,12);calls=errors=0;expectedCamera->SetView(*actualForwards[forward],*actualUps[up]);++scenarios;
        if(errors || calls!=2 || memcmp(storage,expected,sizeof(storage)) || memcmp(&externalUp,&upBefore,12) || memcmp(&externalForward,&forwardBefore,12)) return 1;
    }
    printf("CAMERA_SET_VIEW_BOUNDARY_PASS scenarios=%u\n",scenarios);return 0;
}
