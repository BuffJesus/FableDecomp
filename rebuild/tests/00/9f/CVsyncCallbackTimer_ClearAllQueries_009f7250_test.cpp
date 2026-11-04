#include "fable_display_viewport.h"
#include "fable_d3d9_query.h"
#include <stdio.h>
#include <string.h>
typedef _STL::_List_node<CVsyncCallbackTimer::CPendingQuery> Node;
struct CVsyncCallbackTimerTestAccess {
    static void Set(CVsyncCallbackTimer* p,Node* pending,Node* free){p->PendingQueories._M_node=pending;p->FreeQueoryObjects._M_node=free;}
    static void Run(CVsyncCallbackTimer* p){p->ClearAllQueries();}
};
static unsigned long timerStorage[0x14/4],expectedTimer[0x14/4],queries[32][2],table[3];
static Node pendingHead,freeHead;
static unsigned mutation,releaseResult,used,events[32],plan[32],planUsed;
static bool changed;
static void Add(Node& head,unsigned id){
    Node* node=static_cast<Node*>(malloc(sizeof(Node)));
    if(!node){puts("allocation failed");exit(2);}
    node->_M_data.Query=reinterpret_cast<IDirect3DQuery9*>(queries[id]);node->_M_data.Frame=id;node->_M_data.TimeStamp=id+0.25;
    node->_M_next=&head;node->_M_prev=head._M_prev;head._M_prev->_M_next=node;head._M_prev=node;
}
static unsigned long FABLE_STDCALL Release(IDirect3DQuery9* query){
    unsigned id=reinterpret_cast<unsigned long*>(query)[1];
    if(used>=32)exit(3);events[used++]=id;
    if(!changed){
        if(mutation==1 && id<10){Add(pendingHead,30);changed=true;}
        if(mutation==2 && id<10){Add(freeHead,30);changed=true;}
        if(mutation==3 && id>=10){Add(pendingHead,31);changed=true;}
    }
    return releaseResult==0?0:releaseResult==1?1:0xffffffffUL;
}
static void Model(unsigned* pending,unsigned& pendingCount,unsigned* free,unsigned& freeCount){
    bool once=false;planUsed=0;
    while(pendingCount){
        unsigned id=pending[pendingCount-1];plan[planUsed++]=id;
        if(!once && mutation==1){pending[pendingCount++]=30;once=true;}
        if(!once && mutation==2){free[freeCount++]=30;once=true;}
        --pendingCount;
    }
    while(freeCount){
        plan[planUsed++]=free[freeCount-1];
        if(!once && mutation==3){pending[pendingCount++]=31;once=true;}
        --freeCount;
    }
}
static bool Check(Node& head,const unsigned* values,unsigned count){
    _STL::_List_node_base* node=head._M_next;_STL::_List_node_base* previous=&head;
    for(unsigned i=0;i<count;++i){
        if(node==&head || node->_M_prev!=previous || static_cast<Node*>(node)->_M_data.Query!=reinterpret_cast<IDirect3DQuery9*>(queries[values[i]]))return false;
        previous=node;node=node->_M_next;
    }
    return node==&head && head._M_prev==previous;
}
static void Clean(Node& head){while(head._M_next!=&head){Node* node=static_cast<Node*>(head._M_next);node->_M_unhook();free(node);}}
int main(){
    table[2]=reinterpret_cast<unsigned long>(&Release);
    for(unsigned i=0;i<32;++i){queries[i][0]=reinterpret_cast<unsigned long>(table);queries[i][1]=i;}
    unsigned scenarios=0;
    for(unsigned p=0;p<5;++p)for(unsigned f=0;f<5;++f)for(mutation=0;mutation<4;++mutation)for(releaseResult=0;releaseResult<3;++releaseResult){
        memset(timerStorage,0xa5,sizeof(timerStorage));pendingHead._M_next=pendingHead._M_prev=&pendingHead;freeHead._M_next=freeHead._M_prev=&freeHead;
        unsigned pending[8],free[8],pn=p,fn=f;
        for(unsigned i=0;i<p;++i){pending[i]=i+1;Add(pendingHead,i+1);}
        for(unsigned i=0;i<f;++i){free[i]=i+10;Add(freeHead,i+10);}
        CVsyncCallbackTimer* timer=reinterpret_cast<CVsyncCallbackTimer*>(timerStorage);CVsyncCallbackTimerTestAccess::Set(timer,&pendingHead,&freeHead);
        memcpy(expectedTimer,timerStorage,sizeof(timerStorage));Model(pending,pn,free,fn);used=0;changed=false;
        CVsyncCallbackTimerTestAccess::Run(timer);++scenarios;
        bool passed=used==planUsed && !memcmp(events,plan,used*sizeof(unsigned)) && !memcmp(timerStorage,expectedTimer,sizeof(timerStorage)) && Check(pendingHead,pending,pn) && Check(freeHead,free,fn);
        Clean(pendingHead);Clean(freeHead);
        if(!passed){printf("TIMER_CLEAR_FAIL pending=%u free=%u mutation=%u result=%u\n",p,f,mutation,releaseResult);return 1;}
    }
    printf("TIMER_CLEAR_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
