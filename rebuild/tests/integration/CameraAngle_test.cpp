#include "fable_camera.h"
#ifndef FABLE_CAMERA_ANGLE_ORACLE_HEADER
#define FABLE_CAMERA_ANGLE_ORACLE_HEADER "CameraAngle_oracle.h"
#endif
#include FABLE_CAMERA_ANGLE_ORACLE_HEADER
#include <stdio.h>
#include <string.h>
#include <float.h>
static CCamera expectedCamera;
static unsigned renderCalls,errors;
class ProbeCamera : public CAIGameCameraBase {
public:
    virtual void Update() {++errors;}
    virtual CCamera GetRenderCamera(float t) const {if(t!=1.0f) ++errors;++renderCalls;return expectedCamera;}
};
static bool Same(unsigned long actual,unsigned long expected) {
    if((expected&0x7f800000UL)==0x7f800000UL && (expected&0x007fffffUL)) return (actual&0x7f800000UL)==0x7f800000UL && (actual&0x007fffffUL)!=0;
    if((expected&0x7fffffffUL)==0) return actual==expected;
    if((actual&0x80000000UL)!=(expected&0x80000000UL)) return false;
    unsigned long distance=actual>expected?actual-expected:expected-actual;
    return distance<=2;
}
__declspec(noinline) static float ControlAngle(const CAIGameCameraBase* p) {return p->GetControlAngleXY();}
int main() {
    unsigned oldControl=_control87(0,0);_control87(_PC_53 | _RC_NEAR | _MCW_EM,_MCW_PC | _MCW_RC | _MCW_EM);unsigned scenarios=0;ProbeCamera p;
    for(unsigned i=0;i<sizeof(angleCases)/sizeof(angleCases[0]);++i) {
        memset(&expectedCamera,0xa9,sizeof(expectedCamera));memcpy(&expectedCamera.RHSet.Forward.X,&angleCases[i][0],4);memcpy(&expectedCamera.RHSet.Forward.Y,&angleCases[i][1],4);
        unsigned char before[sizeof(expectedCamera)];memcpy(before,&expectedCamera,sizeof(expectedCamera));
        for(unsigned composed=0;composed<2;++composed) {
            renderCalls=errors=0;float angle=composed?ControlAngle(&p):expectedCamera.GetAngleXY();unsigned long bits;memcpy(&bits,&angle,4);++scenarios;
            if(!Same(bits,angleCases[i][2]) || errors || renderCalls!=composed || memcmp(before,&expectedCamera,sizeof(expectedCamera))) {printf("ANGLE_FAIL case=%u composed=%u actual=%08lx expected=%08lx\n",i,composed,bits,angleCases[i][2]);return 1;}
        }
    }
    _control87(oldControl,_MCW_PC | _MCW_RC | _MCW_EM);printf("CAMERA_ANGLE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
