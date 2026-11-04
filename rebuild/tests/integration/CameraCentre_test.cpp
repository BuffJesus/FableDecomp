#include "fable_world.h"
#include "fable_game_camera_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static CGameCameraManager* expectedManager;
static unsigned recentres,queries,errors;
// The fixture never invokes the inherited angle or render callbacks.
float CCamera::GetAngleXY() const {abort();return 0;}
void CGameCameraManager::RecentreCamera() {if(this!=expectedManager) ++errors;++recentres;}
class ProbeManager : public CGameCameraManager {
public:
    virtual void Update() {abort();}
    virtual CCamera GetRenderCamera(float) const {abort();CCamera c;return c;}
};
class QueryCamera : public CAIGameCameraBase {
public:
    CGameCameraManager* Target;
    bool Result;
    virtual void Update() {abort();}
    virtual CCamera GetRenderCamera(float) const {abort();CCamera c;return c;}
    virtual bool GetCameraManager(const CGameCameraManager**) const {abort();return false;}
    virtual bool GetCameraManager(CGameCameraManager** out) {++queries;*out=Target;return Result;}
};
__declspec(noinline) static bool Query(CAIGameCameraBase* p,CGameCameraManager** out) {return p->GetCameraManager(out);}
__declspec(noinline) static bool ConstQuery(const CAIGameCameraBase* p,const CGameCameraManager** out) {return p->GetCameraManager(out);}
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<256;++pattern) {
        ProbeManager manager;QueryCamera query;query.Target=&manager;query.Result=pattern&1;
        unsigned long worldStorage[104];memset(worldStorage,pattern,sizeof(worldStorage));CWorld* w=reinterpret_cast<CWorld*>(&worldStorage[2]);
        for(unsigned op=0;op<4;++op) {
            w->PCurrentGameCamera=op==3?static_cast<CAIGameCameraBase*>(&query):static_cast<CAIGameCameraBase*>(&manager);
            unsigned long worldBefore[104];unsigned char managerBefore[sizeof(manager)],queryBefore[sizeof(query)];memcpy(worldBefore,worldStorage,sizeof(worldStorage));memcpy(managerBefore,&manager,sizeof(manager));memcpy(queryBefore,&query,sizeof(query));
            expectedManager=&manager;recentres=queries=errors=0;
            CGameCameraManager* out=0;const CGameCameraManager* cout=0;
            if(op==0) {if(!Query(&manager,&out) || out!=&manager) return 1;}
            else if(op==1) {if(!ConstQuery(&manager,&cout) || cout!=&manager) return 2;}
            else w->EACentreCamera(0);
            ++scenarios;
            if(errors || recentres!=unsigned(op>=2) || queries!=unsigned(op==3) || memcmp(worldBefore,worldStorage,sizeof(worldStorage)) || memcmp(managerBefore,&manager,sizeof(manager)) || memcmp(queryBefore,&query,sizeof(query))) return 3;
        }
    }
    printf("CAMERA_CENTRE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
