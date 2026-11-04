#include "fable_system_manager_state.h"
#include <new>
#include <stdio.h>
#include <string.h>

static unsigned long storage[0x180/4], expected[0x180/4];
static unsigned long infos[8][3], replacement[8][3], expectedInfos[8][3];
static unsigned events[80], planned[80], used, plannedUsed, errors;
static unsigned fill, mutation;
static bool running;
static void* baseVTable;
static const unsigned offsets[]={0x50,0x58,0x60,0x6c,0x74,0x7c,0x84,0xf8};
extern "C" void* SystemLifetimeVTable[];
#pragma comment(linker,"/alternatename:_SystemLifetimeVTable=??_7CSystemManager@@6B@")
static void Event(unsigned value) {if(used<80)events[used++]=value;else ++errors;}
static void Plan(unsigned value) {planned[plannedUsed++]=value;}
static unsigned Offset(const void* p) {return reinterpret_cast<const char*>(p)-reinterpret_cast<const char*>(storage);}
CBaseClass::CBaseClass(){Event(1);}
CBaseClass::~CBaseClass(){baseVTable=*reinterpret_cast<void**>(this);Event(10);}
CInitBaseClass::CInitBaseClass():Valid(false){Event(2);}
CInitBaseClass::~CInitBaseClass(){Event(9);}
void CInitBaseClass::Validate() const {}
CWideString::CWideString(){Event(Offset(this)==0xa0?3:4);*reinterpret_cast<unsigned long*>(this)=0x12340000+Offset(this);}
CCharString::CCharString(){Event(5);*reinterpret_cast<unsigned long*>(this)=0x12340000+Offset(this);}
CWideString::~CWideString(){if(*reinterpret_cast<void**>(storage)!=SystemLifetimeVTable)++errors;Event(Offset(this)==0xa0?8:7);*reinterpret_cast<unsigned long*>(this)=0;}
CCharString::~CCharString(){if(*reinterpret_cast<void**>(storage)!=SystemLifetimeVTable)++errors;Event(6);*reinterpret_cast<unsigned long*>(this)=0;}
static void FABLE_FASTCALL Destroy(void* data) {
    unsigned id=reinterpret_cast<unsigned long>(data)-0x1000;
    if(id>=8 || infos[id][0]!=0 || *reinterpret_cast<void**>(storage)!=SystemLifetimeVTable){++errors;return;}
    Event(100+id);
    if(mutation) storage[offsets[id]/4+1]=reinterpret_cast<unsigned long>(replacement[id]);
}
void __cdecl operator delete(void* data) {
    if(!running)return;
    for(unsigned i=0;i<8;++i)if(data==(mutation?replacement[i]:infos[i])){Event(200+i);return;}
    ++errors;
}
int main() {
    unsigned scenarios=0;
    for(fill=0;fill<2;++fill)for(unsigned pattern=0;pattern<4;++pattern)for(mutation=0;mutation<2;++mutation) {
        memset(storage,fill?0xa5:0,sizeof(storage));memcpy(expected,storage,sizeof(storage));used=plannedUsed=errors=0;
        CSystemManager* manager=new(storage) CSystemManager;
        expected[0]=reinterpret_cast<unsigned long>(SystemLifetimeVTable);reinterpret_cast<unsigned char*>(expected)[4]=0;
        for(unsigned i=0;i<8;++i)expected[offsets[i]/4]=expected[offsets[i]/4+1]=0;
        expected[0xa0/4]=0x123400a0;expected[0x164/4]=0x12340164;expected[0x168/4]=0x12340168;
        const unsigned construction[]={1,2,3,4,5};
        if(errors || used!=5 || memcmp(events,construction,sizeof(construction)) || memcmp(storage,expected,sizeof(storage)))return 1;
        for(unsigned i=0;i<8;++i) {
            unsigned mode=(pattern+i)%4;
            infos[i][0]=mode==1?1:mode==2?2:0;infos[i][1]=reinterpret_cast<unsigned long>(&Destroy);infos[i][2]=0x1000+i;
            storage[offsets[i]/4]=mode?0x1000+i:0;storage[offsets[i]/4+1]=mode?reinterpret_cast<unsigned long>(infos[i]):0;
            memset(replacement[i],0x5a,sizeof(replacement[i]));
        }
        memcpy(expectedInfos,infos,sizeof(infos));memcpy(expected,storage,sizeof(storage));
        used=0;Plan(6);Plan(7);
        for(int i=7;i>=0;--i) {
            if(i==6)Plan(8);
            if(storage[offsets[i]/4+1]) {
                --expectedInfos[i][0];
                if(!expectedInfos[i][0]){Plan(100+i);Plan(200+i);}
            }
            expected[offsets[i]/4]=expected[offsets[i]/4+1]=0;
        }
        Plan(9);Plan(10);expected[0xa0/4]=expected[0x164/4]=expected[0x168/4]=0;
        running=true;manager->~CSystemManager();running=false;
        expected[0]=reinterpret_cast<unsigned long>(baseVTable);++scenarios;
        if(errors || used!=plannedUsed || memcmp(events,planned,used*sizeof(unsigned)) || memcmp(storage,expected,sizeof(storage)) || memcmp(infos,expectedInfos,sizeof(infos))) {
            printf("SYSTEM_LIFETIME_FAIL scenario=%u errors=%u\n",scenarios,errors);return 2;
        }
    }
    printf("SYSTEM_LIFETIME_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
