#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_game.h"
#include "fable_iengine.h"
#include "fable_nui_manager.h"
#include "fable_ime.h"
#include "fable_rgb_colour.h"

static unsigned long renderStorage[2][0x4000/4],componentStorage[0x200/4],engineStorage[2][4],vtable[9],interpolationStorage[0x80/4];
static unsigned char expected[2][0x4000],displayStorage[0x300],managerStorage[0x100],frontendStorage[0x100],imeStorage[0x60];
static unsigned scenario,step,engineCalls;
static float expectedTime,expectedWidth,expectedHeight;
static unsigned sequence[32],sequenceSize;
CRenderManager2D* g_FableRenderManager2D_013B8384;
CDisplayManager* g_FableDisplayManager_013B8390;
double g_FableVirtualWidth_013961E8,g_FableVirtualHeight_013961F0;
unsigned long CRenderStateManager::D3DTextureMipMapModeTable[]={1,2,0};
static void require(bool ok){if(!ok){fprintf(stderr,"draw failure scenario=%u step=%u\n",scenario,step);abort();}}
static void note(unsigned kind){require(step<sequenceSize&&sequence[step]==kind);++step;}
static unsigned long& word(unsigned char* data,unsigned offset){return *(unsigned long*)(data+offset);}
static unsigned char* actual(unsigned index){return (unsigned char*)renderStorage[index];}
static void expectedState(unsigned index,unsigned offset,unsigned value){
    unsigned char* data=expected[index];unsigned old=word(data,offset+4);
    if(old==value)return;
    if(!(word(data,offset+8)&word(data,0x2814))){
        word(data,offset+8)|=word(data,0x2814);
        unsigned position=word(data,0x2808)++;
        word(data,8+8*position)=(unsigned)(actual(index)+offset);
        word(data,12+8*position)=old;
    }
    word(data,offset+4)=value;
    if(!data[offset+16]){data[offset+16]=1;unsigned position=word(data,0x280c)++;word(data,0x2008+4*position)=(unsigned)(actual(index)+offset);}
}
CInterpolationInfo::CInterpolationInfo(){note(1);}
double FABLE_FASTCALL GFGetTime(){note(2);return 1.0000001;}
void CRenderManagerCore::SetVWindow(const C2DBoxF& window){
    note(3);require(this==(CRenderManagerCore*)actual(0));
    require((_controlfp(0,0)&_MCW_RC)==_RC_NEAR);
    require(window.left==0&&window.top==0&&window.right==expectedWidth&&window.bottom==expectedHeight);
    require(!memcmp(actual(0),expected[0],0x4000));
    if(scenario&32)g_FableRenderManager2D_013B8384=(CRenderManager2D*)actual(1);
}
C2DBoxF CDisplayManager::GetVVisibleTelevisionScreenArea()const{
    note(4);require(this==(CDisplayManager*)displayStorage);C2DBoxF value;value.Initialise(1,2,3,4);return value;
}
void CRenderManager2D::ClearRenderTarget(const CRGBColour& colour,unsigned long flags){
    note(5);require(this==g_FableRenderManager2D_013B8384&&colour.GetD3DCOLOR()==0xff000000&&flags==0);
}
bool CDisplayManager::BeginRender(){note(6);require(this==(CDisplayManager*)displayStorage);return (scenario&8)!=0;}
NUISystem::CFrontEndManager* FABLE_FASTCALL NUISystem::CFrontEndManager::GetInstance(){note(7);return (CFrontEndManager*)frontendStorage;}
void NUISystem::CFrontEndManager::Draw(CIEngine& engine,CEnginePrimitiveHandle* handle){
    note(8);require(this==(CFrontEndManager*)frontendStorage&&&engine==(CIEngine*)engineStorage[0]&&!handle);
    if(scenario&32)word((unsigned char*)componentStorage,0x58)=(unsigned)engineStorage[1];
}
NUISystem::CManager* FABLE_FASTCALL NUISystem::CManager::GetInstance(){note(9);return (CManager*)managerStorage;}
void NUISystem::CManager::RenderErrorMessage(CIEngine& engine){
    note(10);require(this==(CManager*)managerStorage&&&engine==(CIEngine*)engineStorage[(scenario&32)?1:0]);
}
static void __fastcall EngineRender(void* self,void*,const CEngineInterpolationInfo& info,unsigned long flags){
    note(11);require(self==engineStorage[(scenario&32)?1:0]&&&info==(CEngineInterpolationInfo*)interpolationStorage&&flags==0);++engineCalls;
    expectedState((scenario&32)?1:0,0x2e44,2);
}
void CRenderManager2D::DrawBufferedPolys(){
    note(12);require(this==g_FableRenderManager2D_013B8384);
    require(!memcmp(actual(0),expected[0],0x4000)&&!memcmp(actual(1),expected[1],0x4000));
}
void CRenderManager2D::Render2DDrawList(bool flag){note(13);require(this==g_FableRenderManager2D_013B8384&&flag);}
NUISystem::CIME& FABLE_FASTCALL NUISystem::CIME::Get(){note(14);return *(CIME*)imeStorage;}
void NUISystem::CIME::Render(){note(15);require(this==(CIME*)imeStorage);}
void CDisplayManager::EndRender(){note(16);require(this==(CDisplayManager*)displayStorage);}
bool CDisplayManager::SwapScreens(){note(17);require(this==(CDisplayManager*)displayStorage);return (scenario&8)!=0;}
int main(){
    unsigned modes[]={_RC_NEAR,_RC_DOWN,_RC_UP,_RC_CHOP},cases=0;
    for(scenario=0;scenario!=64;++scenario)for(unsigned rounding=0;rounding!=4;++rounding)for(unsigned dimensions=0;dimensions!=2;++dimensions){
        memset(renderStorage,0xa5,sizeof(renderStorage));memset(componentStorage,0,sizeof(componentStorage));
        for(unsigned index=0;index!=2;++index){
            unsigned char* data=actual(index);word(data,0x2808)=0;word(data,0x280c)=0;word(data,0x2814)=3;
            for(unsigned n=0;n!=2;++n){unsigned offset=n?0x2e44:0x2944;
                word(data,offset+4)=n?((scenario&2)?2:0):((scenario&1)?1:0);
                word(data,offset+8)=(scenario&4)?1:4;data[offset+16]=(scenario&8)!=0;
            }
            memcpy(expected[index],data,0x4000);
        }
        expectedState(0,0x2944,0);
        // Fog is checked at its first callback; mip state is applied to the
        // expected image only once the UI/engine calls have finished below.
        vtable[8]=(unsigned)&EngineRender;engineStorage[0][0]=engineStorage[1][0]=(unsigned)vtable;
        word((unsigned char*)componentStorage,0x58)=(unsigned)engineStorage[0];
        managerStorage[0x9c]=(scenario&16)!=0;g_FableRenderManager2D_013B8384=(CRenderManager2D*)actual(0);g_FableDisplayManager_013B8390=(CDisplayManager*)displayStorage;
        g_FableVirtualWidth_013961E8=dimensions?640.00001:1920.00001;g_FableVirtualHeight_013961F0=dimensions?480.00001:1080.00001;
        _controlfp(modes[rounding],_MCW_RC);
        volatile double time=1.0000001,width=g_FableVirtualWidth_013961E8,height=g_FableVirtualHeight_013961F0;
        expectedTime=(float)time;expectedWidth=(float)width;expectedHeight=(float)height;
        const unsigned first[]={1,2,3,4,5,6,7,8,9};sequenceSize=0;
        for(unsigned n=0;n!=9;++n)sequence[sequenceSize++]=first[n];
        if(scenario&16){sequence[sequenceSize++]=9;sequence[sequenceSize++]=10;}
        const unsigned last[]={11,12,13,14,15,12,13,16,17};for(n=0;n!=9;++n)sequence[sequenceSize++]=last[n];
        step=engineCalls=0;((CNewFrontendGameComponent*)componentStorage)->Draw(*(CEngineInterpolationInfo*)interpolationStorage);
        require(step==sequenceSize&&engineCalls==1&&*(float*)((unsigned char*)componentStorage+0xbc)==expectedTime);
        require((_controlfp(0,0)&_MCW_RC)==_RC_NEAR);++cases;
    }
    printf("NATIVE_FRONTEND_DRAW_PASS cases=%u\n",cases);return 0;
}
