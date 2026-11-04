#include "fable_busy_resource_manager.h"
#include <new>
#include <stdio.h>
#include <string.h>
struct CBusyResourceManagerTestAccess {
    typedef _STL::list<CBusyResourceManager::CPendingQuery> List;
    static List& Free(CBusyResourceManager* p){return p->FreeQueoryObjects;}
    static List& Pending(CBusyResourceManager* p){return p->PendingQueories;}
    static bool Frames(CBusyResourceManager* p){return p->PendingFrame==1 && p->FinishedFrame==0;}
    static IDirect3DDevice9* Device(CBusyResourceManager* p){return p->D3DDevice;}
};
typedef CBusyResourceManagerTestAccess Access;
typedef Access::List List;
static CBusyResourceManager* manager;
static unsigned long vtable[3],queries[64][2];
static unsigned released[64],count,failures;
static bool appendFree,appended;
static void check(bool b){if(!b)++failures;}
static void add(List& list,unsigned id) {
    List::_Node* node=static_cast<List::_Node*>(malloc(sizeof(List::_Node)));
    node->_M_data.Query=reinterpret_cast<IDirect3DQuery9*>(queries[id]);node->_M_data.Frame=id*17;
    node->_M_next=list._M_node;node->_M_prev=list._M_node->_M_prev;
    node->_M_prev->_M_next=node;list._M_node->_M_prev=node;
}
static unsigned long FABLE_STDCALL Release(void* q) {
    unsigned id=static_cast<unsigned long*>(q)[1];check(count<64);released[count++]=id;
    if(appendFree && !appended) {appended=true;add(Access::Free(manager),63);}
    return 123;
}
int main() {
    vtable[2]=reinterpret_cast<unsigned long>(Release);
    for(unsigned i=0;i<64;++i){queries[i][0]=reinterpret_cast<unsigned long>(vtable);queries[i][1]=i;}
    unsigned scenarios=0;
    for(unsigned pending=0;pending<16;++pending)for(unsigned available=0;available<16;++available)for(unsigned mode=0;mode<8;++mode) {
        unsigned long storage[5];memset(storage,(mode&1)?0xa5:0x5a,sizeof(storage));unsigned long untouched=storage[4];
        manager=new(storage) CBusyResourceManager;
        List& p=Access::Pending(manager);List& f=Access::Free(manager);
        check(p._M_node!=f._M_node);check(p._M_node->_M_next==p._M_node && p._M_node->_M_prev==p._M_node);
        check(f._M_node->_M_next==f._M_node && f._M_node->_M_prev==f._M_node);
        check(Access::Frames(manager));check(reinterpret_cast<unsigned long>(Access::Device(manager))==untouched);
        for(unsigned j=0;j<pending;++j)add(p,j);
        for(unsigned k=0;k<available;++k)add(f,k+16);
        count=0;appendFree=(mode&2)!=0 && pending>0;appended=false;
        manager->ClearAllQueries();unsigned cursor=0;
        for(unsigned n=pending;n>0;--n)check(released[cursor++]==n-1);
        if(appendFree)check(released[cursor++]==63);
        for(unsigned m=available;m>0;--m)check(released[cursor++]==m+15);
        check(cursor==count);check(p._M_node->_M_next==p._M_node && p._M_node->_M_prev==p._M_node);
        check(f._M_node->_M_next==f._M_node && f._M_node->_M_prev==f._M_node);
        if(mode&4){manager->ClearAllQueries();check(cursor==count);}
        check(Access::Frames(manager));check(reinterpret_cast<unsigned long>(Access::Device(manager))==untouched);
        free(p._M_node);free(f._M_node);++scenarios;
    }
    printf("BUSY_RESOURCE_BEHAVIOR_%s scenarios=%u failures=%u\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
