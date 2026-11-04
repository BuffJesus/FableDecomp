#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_nui_manager.h"
static int events[12],count;static unsigned char* object;
namespace NUISystem {
CObserver::CObserver(){events[count++]=1;memset((char*)this+4,0xbc,16);}
CObserver::~CObserver(){}
CFrontEndManager::~CFrontEndManager(){}
}
CWideString::CWideString(){events[count++]=(int)((unsigned char*)this-object);*(void**)this=0;}
CWideString::~CWideString(){}
CCharString::~CCharString(){}
namespace _STL {
template<class T,class A> _Deque_base<T,A>::_Deque_base(const A&,size_t){}
template<class T,class A> list<T,A>::list(const A&){events[count++]=0x18;_M_node=(_Node*)malloc(sizeof(_Node));_M_node->_M_next=_M_node;_M_node->_M_prev=_M_node;}
template<class T,class A> list<T,A>::~list(){free(_M_node);}
template<class T,class S> deque<T,S>::deque() : _Deque_base<T,S>(S(),0){events[count++]=0x20;memset(this,0x72,sizeof(*this));}
template list<NUISystem::CActionParam*>::list(const allocator<NUISystem::CActionParam*>&);
template deque<NUISystem::CComponent*>::deque();
}
#define CHECK(x) do{if(!(x)){printf("FAIL %d\n",__LINE__);return 1;}}while(0)
int main(){
 union Storage {double align;unsigned char bytes[0xe8];} storage;
 memset(storage.bytes,0xa5,sizeof(storage.bytes));object=storage.bytes+4;
 NUISystem::CFrontEndManager* p=new(object) NUISystem::CFrontEndManager;
 CHECK(sizeof(*p)==0xe0);CHECK(count==6);
 int expected[]={1,0x14,0x18,0x20,0xac,0xb0};for(int i=0;i<6;++i)CHECK(events[i]==expected[i]);
 CHECK(p->UIActionParams.size()==0);CHECK(p->m_vComponents.size()==0);
 CHECK(p->m_vFileTimes.size()==0);CHECK(p->FileRegionNames.size()==0);CHECK(p->FileRegionMiniMapNames.size()==0);CHECK(p->FilePlayTimes.size()==0);
 CHECK(p->m_mapScreens._M_t._M_node_count==0);CHECK(p->Signatures._M_t._M_node_count==0);
 CHECK(!p->m_pVirtualKeyboard&&!p->m_bStartNewGame&&!p->PScreenToDeactivate&&!p->PScreenToActivate&&!p->GoToKeyboard&&!p->PScreenToDeactivateForStartGame&&!p->PScreenToDeactivateForLoadGame&&!p->m_bQuitFromGame&&!p->InstallationPaused&&!p->FinalInstallationNeverPause);
 CHECK(*(unsigned long*)&p->PGameComponent==0xa5a5a5a5);CHECK(p->BlocksOnUNeeded==0xa5a5a5a5&&p->BlocksOnTNeeded==0xa5a5a5a5&&p->NumberSaveGames==0xa5a5a5a5);
 for(int j=0;j<8;++j)CHECK(((unsigned char*)&p->InstallationPauseStartTime)[j]==0xa5);
 for(int k=0;k<4;++k)CHECK(storage.bytes[k]==0xa5&&storage.bytes[0xe4+k]==0xa5);
 free(p->m_mapScreens._M_t._M_header._M_data);free(p->Signatures._M_t._M_header._M_data);
 p->~CFrontEndManager();puts("FRONTEND_CTOR_PASS");return 0;
}

