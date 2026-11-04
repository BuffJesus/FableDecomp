#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fable_system_manager.h"
typedef CProfileManager::CProfileTreeEntry Entry;typedef _STL::list<Entry> List;typedef List::_Node Node;
CWideString::CWideString() {}
CWideString::~CWideString() {}
static const Entry* expected;static char calls[32];
CProfile::CProfile(const CProfile& value){if(&value!=&expected->Profile)abort();strcat(calls,"p");memcpy(this,&value,sizeof(*this));}
namespace _STL {
 template<> vector<Entry*>::vector(const vector<Entry*>& value):_Vector_base<Entry*,allocator<Entry*> >(allocator<Entry*>()){
  if(&value!=&expected->Children)abort();strcat(calls,"v");size_t n=value.size();_M_start=(Entry**)malloc(n*sizeof(Entry*));memcpy(_M_start,value._M_start,n*sizeof(Entry*));_M_finish=_M_start+n;_M_end_of_storage._M_data=_M_finish;
 }
}
int main(){
 double listRaw[(sizeof(List)+7)/8],entryRaw[(sizeof(Entry)+7)/8];List& list=*reinterpret_cast<List*>(listRaw);Entry& value=*reinterpret_cast<Entry*>(entryRaw);memset(entryRaw,0,sizeof(entryRaw));
 _STL::_List_node_base head;head._M_next=head._M_prev=&head;list._M_node=reinterpret_cast<Node*>(&head);
 Entry* children[2]={reinterpret_cast<Entry*>(0x4000),0};value.Children._M_start=children;value.Children._M_finish=children+2;value.Children._M_end_of_storage._M_data=children+2;value.ID=73;value.Parent=reinterpret_cast<Entry*>(0x1200);value.Profile.Manager=reinterpret_cast<CProfileManager*>(0x5000);expected=&value;calls[0]=0;
 Node* a=(Node*)list.insert(list.end(),value)._M_node;Node* b=(Node*)list.insert(list.end(),value)._M_node;Node* c=(Node*)list.insert(List::iterator(b),value)._M_node;
 if(strcmp(calls,"pvpvpv") || head._M_next!=a || head._M_prev!=b || a->_M_next!=c || c->_M_next!=b || b->_M_next!=&head || b->_M_prev!=c || c->_M_prev!=a || a->_M_prev!=&head)return 1;
 Node* nodes[]={a,b,c};for(int i=0;i<3;++i){Entry& x=nodes[i]->_M_data;if(x.ID!=73 || x.Parent!=value.Parent || x.Profile.Manager!=value.Profile.Manager || x.Children.size()!=2 || x.Children._M_start==children || memcmp(x.Children._M_start,children,sizeof(children)))return 2;free(x.Children._M_start);free(nodes[i]);}
 puts("PROFILE_INSERT_PASS");return 0;
}