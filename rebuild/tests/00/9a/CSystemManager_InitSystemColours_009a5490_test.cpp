#include "fable_system_manager_state.h"
#include "test_support/import_redirect.h"
#include <stdio.h>
#include <string.h>
struct CSystemManagerTestAccess {static void Run(CSystemManager* p){p->InitSystemColours();}};
static const int indices[]={21,15,20,22,16,18,10,27,28,2,3,1,9,19,17,13,14,24,23,4,7,0,5,6,8};
static const unsigned fields[]={0,1,2,3,4,5,6,8,10,7,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
static unsigned long storage[0x180/4];
static unsigned char expected[0x180];
static unsigned profile,calls,errors;
static unsigned long FABLE_STDCALL Colour(int index) {
    if(calls>=25){++errors;return 0;}
    if(index!=indices[calls] || memcmp(storage,expected,sizeof(expected))) ++errors;
    unsigned r=(profile+73*index)&255,g=(3*profile+17*index)&255,b=(5*profile+29*index)&255;
    unsigned long argb=0xff000000UL|(r<<16)|(g<<8)|b;
    memcpy(expected+0x100+4*fields[calls],&argb,4);++calls;
    return ((profile^0x5aUL)<<24)|(b<<16)|(g<<8)|r;
}
extern "C" void* SystemColourImport=reinterpret_cast<void*>(&Colour);
#pragma comment(linker,"/alternatename:__imp__GetSysColor@4=_SystemColourImport")
int main() {
    FableTest::ImportRedirect redirect("GetSysColor",reinterpret_cast<void*>(&Colour));
    unsigned scenarios=0;
    for(profile=0;profile<256;++profile) for(unsigned fill=0;fill<2;++fill) {
        memset(storage,fill?0xa5:0,sizeof(storage));memcpy(expected,storage,sizeof(expected));calls=0;
        CSystemManagerTestAccess::Run(reinterpret_cast<CSystemManager*>(storage));++scenarios;
        if(errors || calls!=25 || memcmp(storage,expected,sizeof(expected))) {printf("SYSTEM_COLOURS_FAIL profile=%u fill=%u errors=%u calls=%u\n",profile,fill,errors,calls);return 1;}
    }
    printf("SYSTEM_COLOURS_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
