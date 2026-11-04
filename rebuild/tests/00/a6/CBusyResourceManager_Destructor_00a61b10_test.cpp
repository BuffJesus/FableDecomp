#include "fable_busy_resource_manager.h"
#include <stdio.h>
#include <string.h>
struct CBusyResourceManagerTestAccess {
 typedef _STL::list<CBusyResourceManager::CPendingQuery> List;
 static List& Free(CBusyResourceManager* p){return p->FreeQueoryObjects;}
 static List& Pending(CBusyResourceManager* p){return p->PendingQueories;}
};
typedef CBusyResourceManagerTestAccess A;typedef A::List List;
static CBusyResourceManager* manager;static bool mutate;static int calls,failures;
static void check(bool b){if(!b)++failures;}
void CBusyResourceManager::ClearAllQueries(){check(this==manager);++calls;if(mutate){List::_Node* h=A::Free(this)._M_node;A::Free(this)._M_node=A::Pending(this)._M_node;A::Pending(this)._M_node=h;}}
static void init(List& list,int count){
 list._M_node=static_cast<List::_Node*>(malloc(16));list._M_node->_M_next=list._M_node;list._M_node->_M_prev=list._M_node;
 for(int i=0;i<count;++i){List::_Node* n=static_cast<List::_Node*>(malloc(16));n->_M_data.Query=0;n->_M_data.Frame=123;n->_M_next=list._M_node;n->_M_prev=list._M_node->_M_prev;n->_M_prev->_M_next=n;list._M_node->_M_prev=n;}
}
int main(){int scenarios=0;
 for(int a=0;a<16;++a)for(int b=0;b<16;++b)for(int action=0;action<4;++action){
  unsigned long storage[5];memset(storage,0xa5,sizeof(storage));manager=reinterpret_cast<CBusyResourceManager*>(storage);init(A::Free(manager),a);init(A::Pending(manager),b);calls=0;mutate=(action&1)!=0;
  List::_Node* first=A::Free(manager)._M_node;List::_Node* second=A::Pending(manager)._M_node;
  if(action<2){manager->~CBusyResourceManager();check(calls==1);check(A::Free(manager)._M_node==(mutate?second:first));check(A::Pending(manager)._M_node==(mutate?first:second));}
  else {A::Free(manager).clear();check(calls==0);check(first->_M_next==first && first->_M_prev==first);A::Free(manager).clear();A::Free(manager).~List();A::Pending(manager).~List();}
  check(storage[2]==0xa5a5a5a5 && storage[3]==0xa5a5a5a5 && storage[4]==0xa5a5a5a5);++scenarios;
 }
 printf("BUSY_DESTRUCTOR_BEHAVIOR_%s scenarios=%d failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
