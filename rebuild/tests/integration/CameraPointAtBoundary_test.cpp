#include "fable_camera.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static CCamera* expectedCamera;static C3DVector expectedDifference,originalUp;static unsigned calls,errors;
static bool Equal(const C3DVector& left,const C3DVector& right) {return memcmp(&left,&right,12)==0;}
C3DVector& C3DVector::AccurateNormalise() {
    if(calls==0) {if(this!=&expectedCamera->RHSet.Up || !Equal(*this,originalUp)) ++errors;*this=C3DVector(0,0,1);}
    else if(calls==1) {if(this!=&expectedCamera->RHSet.Forward || !Equal(*this,expectedDifference)) ++errors;*this=C3DVector(0.25f,0.5f,0.75f);}
    else if(calls==2) {if(this!=&expectedCamera->RHSet.Up || !Equal(*this,C3DVector(-0.1875f,-0.375f,0.4375f))) ++errors;*this=C3DVector(-1,-2,-3);}
    else if(calls==3) {if(this!=&expectedCamera->RHSet.Forward || !Equal(*this,C3DVector(0.25f,0.5f,0.75f))) ++errors;*this=C3DVector(4,5,6);}
    else ++errors;
    ++calls;return *this;
}
int main() {
    unsigned oldControl=_control87(0,0);_control87(_PC_53 | _RC_NEAR | _MCW_EM,_MCW_PC | _MCW_RC | _MCW_EM);unsigned scenarios=0;
    for(unsigned pattern=0;pattern<4;++pattern) for(unsigned kind=0;kind<4;++kind) for(unsigned locked=0;locked<2;++locked) for(unsigned height=0;height<2;++height) {
        unsigned char storage[sizeof(CCamera)+32],expected[sizeof(storage)];memset(storage,pattern*71,sizeof(storage));expectedCamera=reinterpret_cast<CCamera*>(storage+16);expectedCamera->WorldPosition=C3DVector(1,-2,3);expectedCamera->RHSet.Up=C3DVector(1,2,3);expectedCamera->RHSet.Forward=C3DVector(4,5,6);expectedCamera->ViewVecZLocked=locked!=0;expectedCamera->HeightLocked=height!=0;originalUp=expectedCamera->RHSet.Up;C3DVector externalTarget(7,8,9),externalBefore=externalTarget;const C3DVector* targets[]={&externalTarget,&expectedCamera->WorldPosition,&expectedCamera->RHSet.Up,&expectedCamera->RHSet.Forward};C3DVector targetSnapshot=*targets[kind];expectedDifference=targetSnapshot-expectedCamera->WorldPosition;if(locked) expectedDifference.Z=0;
        memcpy(expected,storage,sizeof(storage));C3DVector finalUp(-1,-2,-3),finalForward(4,5,6);memcpy(expected+28,&finalUp,12);memcpy(expected+40,&finalForward,12);calls=errors=0;expectedCamera->PointAt(*targets[kind]);++scenarios;
        if(errors || calls!=4 || memcmp(storage,expected,sizeof(storage)) || !Equal(externalTarget,externalBefore)) return 1;
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_POINT_AT_BOUNDARY_PASS scenarios=%u\n",scenarios);return 0;
}
