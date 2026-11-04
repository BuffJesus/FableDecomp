#include <math.h>
#include <stdio.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
#include "fable_virtual_pixels.h"
double g_FableVirtualWidth_013961E8,g_FableVirtualHeight_013961F0;
static unsigned long systemStorage[sizeof(CSystemManager)/4],displayStorage[0x2c8/4],alternateStorage[0x2c8/4];
static CSystemManager* manager=reinterpret_cast<CSystemManager*>(systemStorage);
static CDisplayManager* display=reinterpret_cast<CDisplayManager*>(displayStorage);
static CDisplayManager* alternate=reinterpret_cast<CDisplayManager*>(alternateStorage);
static int scenario,count,failures,axis;
static long width,height;
static double virtualWidth,virtualHeight;
static void check(bool b){if(!b)++failures;}
CSystemManager* FABLE_FASTCALL GFGetSystemManager() {
    check(count++==0);
    if(scenario&1)manager->PDisplayManager.Data=alternate;
    return manager;
}
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions() {
    check(count++==1);check(this==((scenario&1)?alternate:display));
    if(scenario&2){g_FableVirtualWidth_013961E8=virtualWidth;g_FableVirtualHeight_013961F0=virtualHeight;}
    return C2DExtentsI(width,height);
}
float FABLE_FASTCALL GFRoundVXToNearestPixel(double);
float FABLE_FASTCALL GFRoundVYToNearestPixel(double);
float FABLE_FASTCALL GFAXToVX(float);
int main() {
    int cases=0;
    for(scenario=0;scenario<2048;++scenario)for(axis=0;axis<3;++axis) {
        memset(systemStorage,0x5a,sizeof(systemStorage));memset(displayStorage,0xa5,sizeof(displayStorage));memset(alternateStorage,0x3c,sizeof(alternateStorage));
        manager->PDisplayManager.Data=display;
        width=(scenario%29+1)*16;height=(scenario%23+1)*8;
        virtualWidth=width*((scenario%7)+1.0);virtualHeight=height*((scenario%5)+1.0);
        double initialWidth=virtualWidth+width*2.0,initialHeight=virtualHeight+height*3.0;
        g_FableVirtualWidth_013961E8=initialWidth;g_FableVirtualHeight_013961F0=initialHeight;
        double x=(scenario-1024)*0.375;
        double w=(scenario&2)?virtualWidth:initialWidth,h=(scenario&2)?virtualHeight:initialHeight;
        volatile float expected;
        count=0;float result;
        if(axis==0){double scale=w/width;expected=static_cast<long>(floor(x/scale))*scale;result=GFRoundVXToNearestPixel(x);}
        else if(axis==1){double scale=h/height;expected=static_cast<long>(floor(x/scale))*scale;result=GFRoundVYToNearestPixel(x);}
        else {expected=(w*static_cast<float>(x))/width;result=GFAXToVX(static_cast<float>(x));}
        check(result==expected && count==2);check(manager->PDisplayManager.Data==((scenario&1)?alternate:display));
        for(unsigned i=0;i<sizeof(displayStorage)/4;++i)check(displayStorage[i]==0xa5a5a5a5 && alternateStorage[i]==0x3c3c3c3c);
        ++cases;
    }
    printf("VIRTUAL_COORDINATES_BEHAVIOR_%s scenarios=%d failures=%d\n",failures?"FAIL":"PASS",cases,failures);return failures?1:0;
}
