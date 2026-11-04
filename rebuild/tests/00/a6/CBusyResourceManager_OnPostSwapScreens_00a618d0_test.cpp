#include "fable_busy_resource_manager.h"
#include <stdio.h>
#include <string.h>
struct CBusyResourceManagerTestAccess {
 typedef _STL::list<CBusyResourceManager::CPendingQuery> List;
 static List& Free(CBusyResourceManager* p){return p->FreeQueoryObjects;}
 static List& Pending(CBusyResourceManager* p){return p->PendingQueories;}
 static long& Frame(CBusyResourceManager* p){return p->PendingFrame;}
 static long& Finished(CBusyResourceManager* p){return p->FinishedFrame;}
};
typedef CBusyResourceManagerTestAccess A;typedef A::List List;
static CBusyResourceManager* manager;static unsigned long queries[40][2],qtable[8],device[1],dtable[119];
static int mode,mutation,polls,creates,issues,failures,polled[40],issued;
static long createResult,issueResult;
static void check(bool b){if(!b)++failures;}
static long FABLE_STDCALL Poll(void* q,void* data,unsigned long size,unsigned long flags){
 check(data==0 && size==0 && flags==1);polled[polls]=static_cast<unsigned long*>(q)[1];
 long r=mode==0 || (mode==3 && polls%2==0)?0:mode==1 || mode==3?1:0x80004005;
 ++polls;if(mutation&1)A::Frame(manager)=40+polls;return r;
}
static long FABLE_STDCALL Issue(void* q,unsigned long flags){check(flags==1);++issues;issued=static_cast<unsigned long*>(q)[1];if(mutation&4)A::Frame(manager)=201;return issueResult;}
static long FABLE_STDCALL Create(void* d,int type,void** out){check(d==device && type==8);++creates;*out=queries[39];if(mutation&2)A::Frame(manager)=99;return createResult;}
static void init(List& list){list._M_node=static_cast<List::_Node*>(malloc(16));list._M_node->_M_next=list._M_node;list._M_node->_M_prev=list._M_node;}
static void add(List& list,int id){List::_Node* n=static_cast<List::_Node*>(malloc(16));n->_M_data.Query=reinterpret_cast<IDirect3DQuery9*>(queries[id]);n->_M_data.Frame=100+id;n->_M_next=list._M_node;n->_M_prev=list._M_node->_M_prev;n->_M_prev->_M_next=n;list._M_node->_M_prev=n;}
static void verifyAndFree(List& list,int* ids,int count){
 _STL::_List_node_base* n=list._M_node->_M_next;_STL::_List_node_base* prev=list._M_node;
 for(int i=0;i<count;++i){check(n!=list._M_node && n->_M_prev==prev);List::_Node* value=static_cast<List::_Node*>(n);check(value->_M_data.Query==reinterpret_cast<IDirect3DQuery9*>(queries[ids[i]]));if(ids[i]!=39)check(value->_M_data.Frame==100+ids[i]);prev=n;n=n->_M_next;}
 check(n==list._M_node && list._M_node->_M_prev==prev);
 n=list._M_node->_M_next;while(n!=list._M_node){_STL::_List_node_base* next=n->_M_next;free(n);n=next;}free(list._M_node);
}
int main(){
 qtable[7]=reinterpret_cast<unsigned long>(Poll);qtable[6]=reinterpret_cast<unsigned long>(Issue);device[0]=reinterpret_cast<unsigned long>(dtable);dtable[118]=reinterpret_cast<unsigned long>(Create);
 for(int i=0;i<40;++i){queries[i][0]=reinterpret_cast<unsigned long>(qtable);queries[i][1]=i;}
 const int counts[]={0,1,8,9,10,16};int scenarios=0;
 for(int pc=0;pc<6;++pc)for(int fc=0;fc<3;++fc)for(mode=0;mode<4;++mode)for(int cr=0;cr<3;++cr)for(int ir=0;ir<2;++ir)for(mutation=0;mutation<8;++mutation){
  unsigned long storage[5];manager=reinterpret_cast<CBusyResourceManager*>(storage);init(A::Pending(manager));init(A::Free(manager));A::Frame(manager)=10;A::Finished(manager)=77;manager->SetDevice(reinterpret_cast<IDirect3DDevice9*>(device));
  int pending[40],available[40],np=counts[pc],nf=fc;
  for(int p=0;p<np;++p){pending[p]=p+1;add(A::Pending(manager),p+1);}for(int f=0;f<nf;++f){available[f]=f+20;add(A::Free(manager),f+20);}
  createResult=cr==0?0:cr==1?1:0x80004005;issueResult=ir?0x80004005:0;
  int expectedPolls=0,expectedPolled[40],expectedCreates=0,expectedIssues=0,expectedIssued=-1;long frame=10,finished=77;
  while(np){expectedPolled[expectedPolls]=pending[np-1];bool success=mode==0 || (mode==3 && expectedPolls%2==0);++expectedPolls;if(mutation&1)frame=40+expectedPolls;if(!success && np<9)break;finished=100+pending[np-1];available[nf++]=pending[--np];}
  bool proceed=true;
  if(!nf){expectedCreates=1;if(mutation&2)frame=99;if(createResult){finished=frame-2;proceed=false;}else available[nf++]=39;}
  if(proceed){expectedIssues=1;expectedIssued=available[--nf];for(int j=np;j>0;--j)pending[j]=pending[j-1];pending[0]=expectedIssued;++np;if(mutation&4)frame=201;++frame;}
  polls=creates=issues=0;issued=-1;manager->OnPostSwapScreens();
  check(polls==expectedPolls && creates==expectedCreates && issues==expectedIssues && issued==expectedIssued);for(int k=0;k<polls;++k)check(polled[k]==expectedPolled[k]);check(A::Frame(manager)==frame && A::Finished(manager)==finished);check(storage[4]==reinterpret_cast<unsigned long>(device));
  verifyAndFree(A::Pending(manager),pending,np);verifyAndFree(A::Free(manager),available,nf);++scenarios;
 }
 printf("BUSY_POST_SWAP_BEHAVIOR_%s scenarios=%d failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
