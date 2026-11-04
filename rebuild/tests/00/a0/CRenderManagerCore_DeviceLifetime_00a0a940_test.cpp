#include "fable_render_state_manager.h"
#include "fable_surface.h"
#include "fable_system_font.h"
#include <stdio.h>
#include <string.h>
static unsigned long coreStorage[0x3d20/4], expectedStorage[0x3d20/4], displays[2][0x2c8/4], devices[2][1],vtable[8];
static unsigned calls,errors,mutation,fontResult,capsResult,formatChoice;
static IDirect3DDevice9* inputDevice;
static CRenderManagerCore* Core(){return reinterpret_cast<CRenderManagerCore*>(coreStorage);}
static CRenderManagerCore* Expected(){return reinterpret_cast<CRenderManagerCore*>(expectedStorage);}
static void Check(){if(memcmp(coreStorage,expectedStorage,sizeof(coreStorage)))++errors;}
void CRenderStateManager::Init(IDirect3DDevice9* device){
    if(this!=static_cast<CRenderStateManager*>(Core()) || device!=inputDevice || calls++!=0)++errors;Check();
    if(mutation&1){Core()->DisplayManager=reinterpret_cast<CDisplayManager*>(displays[1]);Expected()->DisplayManager=Core()->DisplayManager;}
    Expected()->PD3DDevice=device;
}
bool CSystemFont::Initialise(){
    if(this!=reinterpret_cast<CSystemFont*>(&Core()->SystemFont) || calls++!=1)++errors;Check();
    reinterpret_cast<unsigned char*>(&Core()->SystemFont)[4]=fontResult;
    reinterpret_cast<unsigned char*>(&Expected()->SystemFont)[4]=fontResult;
    if(mutation&2){Core()->PD3DDevice=reinterpret_cast<IDirect3DDevice9*>(devices[1]);Expected()->PD3DDevice=Core()->PD3DDevice;}
    Expected()->TexturesEnabled=true;Expected()->FixedFunctionPipelineEnabled=true;Expected()->HighestMipmapToRender=0;Expected()->PSelectedIndexBuffer=0;
    return fontResult!=0;
}
static HRESULT FABLE_STDCALL Caps(IDirect3DDevice9* device,D3DCAPS9* caps){
    if(device!=Core()->PD3DDevice || caps!=&Core()->D3DDeviceCaps || calls++!=2)++errors;Check();
    memset(caps,0x39,sizeof(*caps));memset(&Expected()->D3DDeviceCaps,0x39,sizeof(*caps));
    if(mutation&4){Core()->DisplayManager=reinterpret_cast<CDisplayManager*>(displays[1]);Expected()->DisplayManager=Core()->DisplayManager;}
    Expected()->TextureStagesInUse=0;Expected()->Rendering=false;
    return capsResult?static_cast<HRESULT>(0x80004005UL):0;
}
CPixelFormat CSurface::GetPixelFormat() const{
    if(this!=reinterpret_cast<CSurface*>(reinterpret_cast<char*>(Core()->DisplayManager)+0x2c) || calls++!=3)++errors;Check();
    if(mutation&8){Core()->TexturesEnabled=false;Expected()->TexturesEnabled=false;}
    CPixelFormat result;result.FormatIndex=formatChoice?-1:23;
    Expected()->ScreenPixelFormat.FormatIndex=result.FormatIndex;Expected()->XBoxShaderConstantsEnabled=false;
    return result;
}
bool CInitBaseClass::IsValid() const{
    if(this!=reinterpret_cast<CInitBaseClass*>(&Core()->SystemFont) || calls++!=0)++errors;Check();return Valid;
}
void CSystemFont::Uninitialise(){
    if(this!=reinterpret_cast<CSystemFont*>(&Core()->SystemFont) || calls++!=1)++errors;Check();
    if(mutation){reinterpret_cast<unsigned char*>(&Core()->SystemFont)[4]=0;reinterpret_cast<unsigned char*>(&Expected()->SystemFont)[4]=0;}
}
int main(){
    vtable[7]=reinterpret_cast<unsigned long>(&Caps);devices[0][0]=devices[1][0]=reinterpret_cast<unsigned long>(vtable);
    unsigned scenarios=0;
    for(unsigned device=0;device<2;++device)for(fontResult=0;fontResult<2;++fontResult)for(capsResult=0;capsResult<2;++capsResult)
    for(formatChoice=0;formatChoice<2;++formatChoice)for(mutation=0;mutation<16;++mutation){
        memset(coreStorage,0xa5,sizeof(coreStorage));Core()->DisplayManager=reinterpret_cast<CDisplayManager*>(displays[0]);
        memcpy(expectedStorage,coreStorage,sizeof(coreStorage));calls=errors=0;inputDevice=reinterpret_cast<IDirect3DDevice9*>(devices[device]);
        Core()->Init(inputDevice);Check();++scenarios;
        if(errors || calls!=4){printf("RENDER_CORE_INIT_FAIL scenario=%u errors=%u calls=%u\n",scenarios,errors,calls);return 1;}
    }
    for(unsigned fill=0;fill<16;++fill)for(unsigned valid=0;valid<2;++valid)for(mutation=0;mutation<2;++mutation){
        memset(coreStorage,fill*17,sizeof(coreStorage));reinterpret_cast<unsigned char*>(&Core()->SystemFont)[4]=valid;
        memcpy(expectedStorage,coreStorage,sizeof(coreStorage));calls=errors=0;Core()->Uninit();Check();++scenarios;
        if(errors || calls!=1+valid){printf("RENDER_CORE_UNINIT_FAIL scenario=%u\n",scenarios);return 1;}
    }
    printf("RENDER_CORE_RESET_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
