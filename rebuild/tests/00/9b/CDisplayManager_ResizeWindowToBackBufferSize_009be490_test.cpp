#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include <stdio.h>
#include <string.h>
struct tagPOINT { long x,y; };
static unsigned char storage[0x2c8],systemStorage[0x180];
static CDisplayManager* display=reinterpret_cast<CDisplayManager*>(storage);
static CSystemManager* systemManager=reinterpret_cast<CSystemManager*>(systemStorage);
static int scenario,count,failures;
static void check(bool ok) { if (!ok) ++failures; }
static int FABLE_STDCALL Metric(int id) {
    const int ids[3]={32,33,4}; check(count<3 && id==ids[count]); ++count;
    return id==32 ? (scenario%17)-8 : id==33 ? (scenario%13)-6 : (scenario%23)-11;
}
static int FABLE_STDCALL Client(HWND__* window,tagPOINT* p) {
    check(count++==3);check(window==reinterpret_cast<HWND__*>(0x12340000));check(p->x==0 && p->y==0);
    if (scenario&8) {p->x=-153;p->y=271;}
    if (scenario&16) {display->BackBufferDimensionsStorage_[0]=-77;display->BackBufferDimensionsStorage_[1]=1999;display->WindowHandle=reinterpret_cast<HWND__*>(0x56780000);}
    return (scenario&32)!=0;
}
static int FABLE_STDCALL Move(HWND__* window,int x,int y,int width,int height,int repaint) {
    check(count++==4);check(window==reinterpret_cast<HWND__*>((scenario&16)?0x56780000:0x12340000));
    check(x==((scenario&8)?-153:0) && y==((scenario&8)?271:0));
    check(width==((scenario&16)?-77:scenario*3-700)+2*((scenario%17)-8));
    check(height==((scenario&16)?1999:scenario*2-400)+2*((scenario%13)-6)+(scenario%23)-11);
    check(repaint==1);return (scenario&64)!=0;
}
extern "C" void* ResizeMetricImport=reinterpret_cast<void*>(Metric);
extern "C" void* ResizeClientImport=reinterpret_cast<void*>(Client);
extern "C" void* ResizeMoveImport=reinterpret_cast<void*>(Move);
#pragma comment(linker,"/alternatename:__imp__GetSystemMetrics@4=_ResizeMetricImport")
#pragma comment(linker,"/alternatename:__imp__ClientToScreen@8=_ResizeClientImport")
#pragma comment(linker,"/alternatename:__imp__MoveWindow@24=_ResizeMoveImport")
int main() {
    for (scenario=0;scenario<4096;++scenario) {
        memset(storage,0xa5,sizeof(storage));memset(systemStorage,0x5a,sizeof(systemStorage));
        display->SystemManager=systemManager;display->ExclusiveMode=(scenario&1)!=0;
        systemManager->AttachedToExternalWindow=(scenario&2)!=0;
        display->WindowHandle=reinterpret_cast<HWND__*>(0x12340000);
        display->BackBufferDimensionsStorage_[0]=scenario*3-700;display->BackBufferDimensionsStorage_[1]=scenario*2-400;
        unsigned char expected[sizeof(storage)],systemBefore[sizeof(systemStorage)];memcpy(expected,storage,sizeof(storage));memcpy(systemBefore,systemStorage,sizeof(systemStorage));
        bool active=(scenario&3)==0;
        if(active && (scenario&16)) {CDisplayManager* e=reinterpret_cast<CDisplayManager*>(expected);e->BackBufferDimensionsStorage_[0]=-77;e->BackBufferDimensionsStorage_[1]=1999;e->WindowHandle=reinterpret_cast<HWND__*>(0x56780000);}
        count=0;display->ResizeWindowToBackBufferSize();check(count==(active?5:0));check(memcmp(expected,storage,sizeof(storage))==0);check(memcmp(systemBefore,systemStorage,sizeof(systemStorage))==0);
    }
    printf("DISPLAY_RESIZE_BEHAVIOR_%s scenarios=4096 failures=%d\n",failures?"FAIL":"PASS",failures);return failures?1:0;
}
