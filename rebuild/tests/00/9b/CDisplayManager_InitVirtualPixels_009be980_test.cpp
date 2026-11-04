#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <limits.h>
double FableVirtualPixelScaleX_013ca800,FableVirtualPixelScaleY_013ca808;
double g_FableVirtualWidth_013961E8,g_FableVirtualHeight_013961F0;
struct CDisplayManagerTestAccess { static void Run(CDisplayManager* p) {p->InitVirtualPixels();} };
static int failures;
static void check(bool ok){if(!ok)++failures;}
static void axis(long dimension,double scale,double total) {
    if (!dimension) {check(!_finite(scale) && scale>0 && _isnan(total));return;}
    // Independent integer floor: C++ division truncates toward zero.
    long quotient=65536/dimension;
    if(dimension<0 && 65536%dimension) --quotient;
    check(scale==static_cast<double>(quotient));
    check(total==static_cast<double>(dimension)*quotient);
}
int main() {
    const long dimensions[]={0,1,-1,2,-2,3,-3,7,-7,320,480,640,768,1024,1080,1920,4096,65535,65536,65537,-65535,-65536,-65537,LONG_MAX,LONG_MIN};
    unsigned long storage[0x2c8/4];unsigned char expected[sizeof(storage)];int scenarios=0;
    for(int a=0;a<25;++a)for(int b=0;b<25;++b)for(int fill=0;fill<2;++fill) {
        memset(storage,fill?0xa5:0,sizeof(storage));CDisplayManager* d=reinterpret_cast<CDisplayManager*>(storage);
        d->RenderTargetDimensionsStorage_[0]=dimensions[a];d->RenderTargetDimensionsStorage_[1]=dimensions[b];
        memcpy(expected,storage,sizeof(storage));reinterpret_cast<CDisplayManager*>(expected)->VirtualCoordsResolutionIndependant=true;
        FableVirtualPixelScaleX_013ca800=-42;FableVirtualPixelScaleY_013ca808=123;
        g_FableVirtualWidth_013961E8=777;g_FableVirtualHeight_013961F0=-999;
        CDisplayManagerTestAccess::Run(d);++scenarios;
        axis(dimensions[a],FableVirtualPixelScaleX_013ca800,g_FableVirtualWidth_013961E8);
        axis(dimensions[b],FableVirtualPixelScaleY_013ca808,g_FableVirtualHeight_013961F0);
        check(memcmp(expected,storage,sizeof(storage))==0);
    }
    printf("DISPLAY_VIRTUAL_PIXELS_BEHAVIOR_%s scenarios=%d failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
