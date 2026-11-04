#include <new>
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_device_reset_callback.h"
#include <stdio.h>
#include <string.h>
typedef _STL::_List_node<CDeviceResetCallback*> Node;
struct CDisplayManagerTestAccess {
    static void Pre(CDisplayManager* p){p->PreDeviceReset();}
    static bool Post(CDisplayManager* p){return p->PostDeviceReset();}
};
static unsigned long display[0x2c8/4], expected[0x2c8/4], systemStorage[2][0x180/4],render[8],timer[16],device[4];
static Node head,alternate,nodes[4],alternateNodes[2];
static unsigned mutation,failAt,used,errors,plannedUsed,events[20],planned[20];
static bool systemResult;
static CDisplayManager* Display(){return reinterpret_cast<CDisplayManager*>(display);}
static void Event(unsigned id){if(used<20)events[used++]=id;else ++errors;}
static void Plan(unsigned id){planned[plannedUsed++]=id;}
static void SwitchHead(){Display()->DeviceResetCallbacks._M_node=&alternate;reinterpret_cast<CDisplayManager*>(expected)->DeviceResetCallbacks._M_node=&alternate;}
class Probe:public CDeviceResetCallback {
public:
    unsigned id;
    void Mutate(){
        if(mutation==1 && id==0)nodes[0]._M_next=&head;
        if(mutation==2){Display()->SystemManager=reinterpret_cast<CSystemManager*>(systemStorage[1]);reinterpret_cast<CDisplayManager*>(expected)->SystemManager=Display()->SystemManager;}
    }
    virtual void OnPreDeviceReset(){Event(100+id);Mutate();}
    virtual bool OnPostDeviceReset(){Event(200+id);Mutate();return id!=failAt;}
};
static Probe probes[6];
void CRenderManagerCore::Uninit(){if(this!=reinterpret_cast<CRenderManagerCore*>(render))++errors;Event(1);}
void CRenderManagerCore::Init(IDirect3DDevice9* d){if(this!=reinterpret_cast<CRenderManagerCore*>(render)||d!=reinterpret_cast<IDirect3DDevice9*>(device))++errors;Event(4);if(mutation==3)SwitchHead();}
void CVsyncCallbackTimer::OnPreDeviceReset(){if(this!=reinterpret_cast<CVsyncCallbackTimer*>(timer))++errors;Event(2);if(mutation==3)SwitchHead();}
void CSystemManager::OnPreDeviceReset(){if(this!=Display()->SystemManager)++errors;Event(3);}
bool CSystemManager::OnPostDeviceReset(){if(this!=Display()->SystemManager)++errors;Event(5);return systemResult;}
static void List(Node& sentinel,Node* list,unsigned length,unsigned firstId) {
    sentinel._M_next=length?list:&sentinel;sentinel._M_prev=length?list+length-1:&sentinel;
    for(unsigned i=0;i<length;++i){list[i]._M_next=i+1<length?list+i+1:&sentinel;list[i]._M_prev=i?list+i-1:&sentinel;list[i]._M_data=&probes[firstId+i];}
}
int main() {
    for(unsigned i=0;i<6;++i)probes[i].id=i;
    unsigned scenarios=0;
    for(unsigned post=0;post<2;++post)for(unsigned renderMode=0;renderMode<3;++renderMode)
    for(unsigned length=0;length<5;++length)for(failAt=0;failAt<7;++failAt)
    for(mutation=0;mutation<4;++mutation)for(unsigned result=0;result<2;++result) {
        memset(display,0xa5,sizeof(display));used=plannedUsed=errors=0;systemResult=result!=0;
        new (&Display()->PRenderManager) boost::scoped_ptr<CRenderManager>(reinterpret_cast<CRenderManager*>(renderMode?reinterpret_cast<CRenderManager2D*>(render):0));
        Display()->UsingRenderManager=renderMode==2;new (&Display()->VsyncCallbackTimer) boost::scoped_ptr<CVsyncCallbackTimer>(reinterpret_cast<CVsyncCallbackTimer*>(reinterpret_cast<CVsyncCallbackTimer*>(timer)));
        Display()->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(device);Display()->SystemManager=reinterpret_cast<CSystemManager*>(systemStorage[0]);
        List(head,nodes,length,0);List(alternate,alternateNodes,2,4);Display()->DeviceResetCallbacks._M_node=&head;memcpy(expected,display,sizeof(display));
        if(post){if(renderMode==2)Plan(4);}else{if(renderMode)Plan(1);Plan(2);}
        bool useAlternate=mutation==3 && (!post || renderMode==2),success=true;
        unsigned count=useAlternate?2:length,first=useAlternate?4:0;
        for(unsigned i=0;i<count;++i) {
            unsigned id=first+i;Plan((post?200:100)+id);
            if(post && id==failAt){success=false;break;}
            if(mutation==1 && id==0)break;
        }
        if(!post)Plan(3);else if(success)Plan(5);
        bool actual=true;
        if(post)actual=CDisplayManagerTestAccess::Post(Display());else CDisplayManagerTestAccess::Pre(Display());
        ++scenarios;
        if(errors || actual!=success || used!=plannedUsed || memcmp(events,planned,used*sizeof(unsigned)) || memcmp(display,expected,sizeof(display))) {
            printf("DEVICE_RESET_PHASES_FAIL scenario=%u errors=%u actual=%u expected=%u\n",scenarios,errors,actual,success);return 1;
        }
    }
    printf("DEVICE_RESET_PHASES_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
