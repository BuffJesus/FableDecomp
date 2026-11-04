#include "fable_camera.h"
#include <stdio.h>
#include <string.h>
class CEnginePrimitivePrepareInfo {};
static unsigned errors,renderCalls,angleCalls;
static unsigned char expectedCamera[0x38];
static float expectedAngle;
class ProbeCamera : public CAIGameCameraBase {
public:
    virtual void Update() {++errors;}
    virtual CCamera GetRenderCamera(float t) const {if(t!=1.0f) ++errors;++renderCalls;CCamera c;memcpy(&c,expectedCamera,sizeof(c));return c;}
};
float CCamera::GetAngleXY() const {if(memcmp(this,expectedCamera,sizeof(*this))) ++errors;++angleCalls;return expectedAngle;}
__declspec(noinline) static bool Run(CAIGameCameraBase* p,unsigned op,const C3DVector& pos,float radius,CEnginePrimitivePrepareInfo& info) {
    CGameCameraManager* out=reinterpret_cast<CGameCameraManager*>(0x12345678);
    const CGameCameraManager* cout=reinterpret_cast<const CGameCameraManager*>(0x87654321);
    switch(op) {
    case 0:p->PreparePrimitivesForRendering(info);break;
    case 1:if(p->IsPosInViewingFrustrum(pos)) return false;break;
    case 2:if(p->Is2DAreaInViewingFrustrum(pos,radius)) return false;break;
    case 3:if(p->IsVolumeInViewingFrustrum(pos,radius)) return false;break;
    case 4:if(p->IsLineOfSightToPos(pos)) return false;break;
    case 5:if(p->GetControlAngleXY()!=expectedAngle) return false;break;
    case 6:p->SetChangeInControlDirection();break;
    case 7:if(static_cast<const CAIGameCameraBase*>(p)->GetCameraManager(&cout) || cout!=reinterpret_cast<const CGameCameraManager*>(0x87654321)) return false;break;
    case 8:if(p->GetCameraManager(&out) || out!=reinterpret_cast<CGameCameraManager*>(0x12345678)) return false;break;
    }
    return true;
}
int main() {
    ProbeCamera p;CEnginePrimitivePrepareInfo info;unsigned scenarios=0;
    for(unsigned value=0;value<256;++value) {
        memset(expectedCamera,value,sizeof(expectedCamera));expectedAngle=static_cast<float>(value)-128.0f;
        C3DVector pos(static_cast<float>(value),-static_cast<float>(value),value*0.5f);C3DVector before=pos;
        unsigned char cameraBefore[sizeof(p)];memcpy(cameraBefore,&p,sizeof(p));
        for(unsigned op=0;op<9;++op) {
            errors=renderCalls=angleCalls=0;
            if(!Run(&p,op,pos,value*0.25f,info) || errors || renderCalls!=unsigned(op==5) || angleCalls!=unsigned(op==5) || memcmp(&pos,&before,sizeof(pos)) || memcmp(&p,cameraBefore,sizeof(p))) return 1;
            ++scenarios;
        }
    }
    printf("CAMERA_BASE_PREFIX_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
