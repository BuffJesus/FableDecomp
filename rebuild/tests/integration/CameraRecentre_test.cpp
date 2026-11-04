#include "fable_world.h"
#include "fable_game_camera_manager.h"
#include "fable_camera_mode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
float CCamera::GetAngleXY() const {abort();return 0;}
struct CGameCameraManagerTestAccess {
    static void Set(CGameCameraManager* m,NCameraModes::CBase* data,CCPPointerInfo* info) {m->CurrentMode.Data=data;m->CurrentMode.Info=info;}
    static unsigned long Offset(CGameCameraManager* m) {return reinterpret_cast<char*>(&m->CurrentMode)-reinterpret_cast<char*>(m);}
};
static NCameraModes::CBase* expectedMode;
static unsigned calls,queries,errors;
static void FABLE_FASTCALL ModeRecentre(NCameraModes::CBase* mode,void*) {if(mode!=expectedMode) ++errors;++calls;}
static void FABLE_FASTCALL UnusedDelete(void*) {abort();}
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
int main() {
    unsigned scenarios=0;
    for(unsigned pattern=0;pattern<256;++pattern)
    for(unsigned live=0;live<2;++live) {
        ProbeManager a,b;QueryCamera query;query.Target=&b;query.Result=pattern&1;
        if(CGameCameraManagerTestAccess::Offset(&a)!=0x98) return 1;
        void* modeTable[17];for(unsigned i=0;i<17;++i) modeTable[i]=reinterpret_cast<void*>(0xffffffe0);modeTable[16]=reinterpret_cast<void*>(&ModeRecentre);
        unsigned long modeA[4],modeB[4];memset(modeA,pattern,sizeof(modeA));memset(modeB,255-pattern,sizeof(modeB));modeA[1]=modeB[1]=reinterpret_cast<unsigned long>(modeTable);
        NCameraModes::CBase* pa=reinterpret_cast<NCameraModes::CBase*>(&modeA[1]);NCameraModes::CBase* pb=reinterpret_cast<NCameraModes::CBase*>(&modeB[1]);
        CCPPointerInfo info(&UnusedDelete,pa);info.RefCount=pattern;
        CGameCameraManagerTestAccess::Set(&a,live?pa:0,&info);CGameCameraManagerTestAccess::Set(&b,live?pb:0,&info);
        for(unsigned op=0;op<3;++op) {
            unsigned long worldStorage[104];memset(worldStorage,pattern,sizeof(worldStorage));CWorld* w=reinterpret_cast<CWorld*>(&worldStorage[2]);w->PCurrentGameCamera=op==2?static_cast<CAIGameCameraBase*>(&query):static_cast<CAIGameCameraBase*>(&a);
            unsigned char beforeA[sizeof(a)],beforeB[sizeof(b)],beforeQuery[sizeof(query)];unsigned long beforeWorld[104],beforeMA[4],beforeMB[4];CCPPointerInfo beforeInfo=info;
            memcpy(beforeA,&a,sizeof(a));memcpy(beforeB,&b,sizeof(b));memcpy(beforeQuery,&query,sizeof(query));memcpy(beforeWorld,worldStorage,sizeof(worldStorage));memcpy(beforeMA,modeA,sizeof(modeA));memcpy(beforeMB,modeB,sizeof(modeB));
            expectedMode=op==2?pb:pa;calls=queries=errors=0;
            if(op==0) a.RecentreCamera();else w->EACentreCamera(0);++scenarios;
            if(calls!=live || queries!=unsigned(op==2) || errors || memcmp(beforeA,&a,sizeof(a)) || memcmp(beforeB,&b,sizeof(b)) || memcmp(beforeQuery,&query,sizeof(query)) || memcmp(beforeWorld,worldStorage,sizeof(worldStorage)) || memcmp(beforeMA,modeA,sizeof(modeA)) || memcmp(beforeMB,modeB,sizeof(modeB)) || memcmp(&beforeInfo,&info,sizeof(info))) return 2;
        }
        CGameCameraManagerTestAccess::Set(&a,0,0);CGameCameraManagerTestAccess::Set(&b,0,0);
    }
    printf("CAMERA_RECENTRE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
