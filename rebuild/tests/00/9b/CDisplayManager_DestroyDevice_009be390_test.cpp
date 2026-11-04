#include "fable_surface.h"
#include <stdio.h>
#include <string.h>
struct CDisplayManagerTestAccess {static void Run(CDisplayManager* p){p->DestroyDevice();}};
static unsigned long storage[0x2c8/4],deviceA[1],deviceB[1],vtable[3];
static CDisplayManager* display=reinterpret_cast<CDisplayManager*>(storage);
static int scenario,count,failures;
static void check(bool b){if(!b)++failures;}
void CSurface::Uninitialise() {
    check(this==reinterpret_cast<CSurface*>(reinterpret_cast<char*>(display)+(count?0x3c:0x2c)));
    check(count<2);++count;
    if(count==1 && (scenario&2)) display->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(deviceB);
    if(count==2 && (scenario&4)) display->D3DDevice=0;
    if(count==2 && (scenario&8)) display->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(deviceA);
}
static unsigned long FABLE_STDCALL Release(void* device) {
    check(count++==2);check(device==display->D3DDevice);
    if(scenario&16)display->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(deviceB);
    return scenario;
}
int main() {
    vtable[2]=reinterpret_cast<unsigned long>(Release);deviceA[0]=deviceB[0]=reinterpret_cast<unsigned long>(vtable);
    for(scenario=0;scenario<512;++scenario) {
        memset(storage,scenario>>5,sizeof(storage));display->D3DDevice=(scenario&1)?reinterpret_cast<IDirect3DDevice9*>(deviceA):0;
        unsigned char expected[sizeof(storage)];memcpy(expected,storage,sizeof(storage));CDisplayManager* e=reinterpret_cast<CDisplayManager*>(expected);
        if(scenario&2)e->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(deviceB);
        if(scenario&4)e->D3DDevice=0;
        if(scenario&8)e->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(deviceA);
        bool release=e->D3DDevice!=0;
        if(release && (scenario&16))e->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(deviceB);
        count=0;CDisplayManagerTestAccess::Run(display);
        check(count==(release?3:2));check(memcmp(expected,storage,sizeof(storage))==0);
    }
    printf("DISPLAY_DESTROY_BEHAVIOR_%s scenarios=512 failures=%d\n",failures?"FAIL":"PASS",failures);return failures?1:0;
}
