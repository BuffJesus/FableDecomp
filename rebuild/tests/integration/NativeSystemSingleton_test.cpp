#include "fable_gfuninitialise.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system_singleton_aliases.h"
#include "system_singleton_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* SystemVTable[];
extern "C" void* __fastcall BaseCtor(void*);
extern "C" void __fastcall BaseDtor(void*);
extern "C" void* __fastcall WideCtor(void*);
extern "C" void* __fastcall CharCtor(void*);
extern "C" void __fastcall WideDtor(void*);
extern "C" void __fastcall CharDtor(void*);
static const unsigned offsets[]={0x50,0x58,0x60,0x6c,0x74,0x7c,0x84,0xf8};
struct Event {unsigned kind,offset,instances;unsigned char state[0x180];};
static Event events[80],construction[80],destruction[80];
static unsigned used,errors,mutation,scenario,registrations;
static unsigned infos[8][3],replacement[8][3],expectedInfos[8][3],expectedReplacement[8][3];
static unsigned char expectedState[0x180];
static bool running;
static void (__cdecl *exitCallback)();
static unsigned Offset(const void* p){return (unsigned)p-(unsigned)&GSystemManager;}
static void Record(unsigned kind,unsigned offset){
    if(used>=80){++errors;return;}Event& e=events[used++];e.kind=kind;e.offset=offset;e.instances=CSystemManager::NoInstances;memcpy(e.state,&GSystemManager,0x180);
}
extern "C" int __cdecl atexit(void (__cdecl *callback)()) {
    // CRT also registers housekeeping before user global construction.
    if(!*(void**)&GSystemManager)return 0;
    exitCallback=callback;++registrations;Record(6,0);return 0;
}
CBaseClass::CBaseClass(){Record(1,Offset(this));}
CBaseClass::~CBaseClass(){Record(10,Offset(this));}
CInitBaseClass::CInitBaseClass():Valid(false){Record(2,Offset(this));}
CInitBaseClass::~CInitBaseClass(){Record(9,Offset(this));}
void CInitBaseClass::Validate() const {}
CWideString::CWideString(){*(unsigned*)this=0x12340000+Offset(this);Record(Offset(this)==0xa0?3:4,Offset(this));}
CCharString::CCharString(){*(unsigned*)this=0x12340000+Offset(this);Record(5,Offset(this));}
CWideString::~CWideString(){*(unsigned*)this=0;Record(Offset(this)==0xa0?8:7,Offset(this));}
CCharString::~CCharString(){*(unsigned*)this=0;Record(11,Offset(this));}
static void FABLE_FASTCALL DeleteData(void* data){
    unsigned i=(unsigned)data-0x1000;if(i>=8){++errors;return;}Record(100+i,offsets[i]);
    if(mutation)*(void**)((unsigned char*)&GSystemManager+offsets[i]+4)=replacement[i];
}
void __cdecl operator delete(void* data){
    if(!running)return;
    for(unsigned i=0;i<8;++i)if(data==(mutation?replacement[i]:infos[i])){Record(200+i,offsets[i]);memset(data,0x95,12);return;}
    ++errors;
}
static void Populate(){
    memset(infos,0,sizeof(infos));memset(replacement,0x5a,sizeof(replacement));
    for(unsigned i=0;i<8;++i){
        unsigned mode=(scenario+i)%5;infos[i][0]=mode==1?1:mode==2?2:mode==3?0:0xffffffff;infos[i][1]=(unsigned)DeleteData;infos[i][2]=0x1000+i;
        unsigned* pair=(unsigned*)((unsigned char*)&GSystemManager+offsets[i]);pair[0]=mode?0x1000+i:0;pair[1]=mode?(unsigned)infos[i]:0;
    }
    CSystemManager::NoInstances=(unsigned char)(scenario*37);used=0;
}
int main(int argc,char** argv){
    if(argc!=3||registrations!=1||!exitCallback||used!=6||errors){printf("startup argc=%d registrations=%u events=%u errors=%u\n",argc,registrations,used,errors);for(unsigned i=0;i<used;++i)printf("event %u offset=%x\n",events[i].kind,events[i].offset);return 2;}
    scenario=atoi(argv[1]);mutation=atoi(argv[2]);
    if(CSystemManager::NoInstances||GFGetSystemManager()!=&GSystemManager)return 3;
    unsigned char expected[0x180]={0};*(void**)expected=SystemVTable;*(unsigned*)(expected+0xa0)=0x123400a0;*(unsigned*)(expected+0x164)=0x12340164;*(unsigned*)(expected+0x168)=0x12340168;
    if(memcmp(expected,&GSystemManager,sizeof(expected)))return 4;
    unsigned ctorEvents=used;memcpy(construction,events,used*sizeof(Event));void (__cdecl *compiledExit)()=exitCallback;
    Populate();running=true;compiledExit();running=false;
    unsigned dtorEvents=used;memcpy(destruction,events,used*sizeof(Event));memcpy(expectedState,&GSystemManager,0x180);memcpy(expectedInfos,infos,sizeof(infos));memcpy(expectedReplacement,replacement,sizeof(replacement));
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned fix=retailFixups[i][0],target=retailFixups[i][1],kind=retailFixups[i][2],value=0;
        switch(target){
        case 0x13ca618:value=(unsigned)&GSystemManager;break;
        case 0x122a680:value=(unsigned)(code+32);break;
        case 0x9a7440:value=(unsigned)(code+64);break;
        case 0x9a5800:value=(unsigned)(code+192);break;
        case 0x4012bc:value=(unsigned)atexit;break;
        case 0x129affc:value=(unsigned)SystemVTable;break;
        case 0x99a310:value=(unsigned)BaseCtor;break;
        case 0x99a410:value=(unsigned)BaseDtor;break;
        case 0x99aed0:value=(unsigned)WideCtor;break;
        case 0x99e4b0:value=(unsigned)CharCtor;break;
        case 0x99b510:value=(unsigned)WideDtor;break;
        case 0x99eae0:value=(unsigned)CharDtor;break;
        case 0xbfe9bc:value=(unsigned)(void (__cdecl *)(void*))operator delete;break;
        default:return 6;
        }
        *(unsigned*)(code+fix)=kind?value-(unsigned)(code+fix+4):value;
    }
    memset(&GSystemManager,0,0x180);CSystemManager::NoInstances=0;used=0;registrations=0;
    ((void (__cdecl *)())code)();
    if(registrations!=1||exitCallback!=(void (__cdecl *)())(code+32)||used!=ctorEvents||memcmp(events,construction,used*sizeof(Event)))return 7;
    Populate();running=true;exitCallback();running=false;
    if(errors||used!=dtorEvents||memcmp(events,destruction,used*sizeof(Event))||memcmp(&GSystemManager,expectedState,0x180)||memcmp(infos,expectedInfos,sizeof(infos))||memcmp(replacement,expectedReplacement,sizeof(replacement)))return 8;
    printf("NATIVE_SYSTEM_SINGLETON_PASS scenario=%u mutation=%u snapshots=%u\n",scenario,mutation,ctorEvents+dtorEvents);return 0;
}
