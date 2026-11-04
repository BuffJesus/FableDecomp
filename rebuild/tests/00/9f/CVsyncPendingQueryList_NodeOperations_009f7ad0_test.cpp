#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>
void* __cdecl operator new(size_t,void* p) throw(){return p;}
typedef CVsyncCallbackTimer::CPendingQuery Query;
typedef _STL::list<Query> List;
typedef List::_Node Node;
namespace _STL {
template<> void list<Query>::push_front(const Query&);
template<> void list<Query>::push_back(const Query&);
}
struct Access:List {static Node* Create(List* list,const Query& value){return static_cast<Access*>(list)->_M_create_node(value);}};
static unsigned errors;
static void Check(Node& head,Query* expected,unsigned count){
 _STL::_List_node_base* previous=&head;_STL::_List_node_base* node=head._M_next;
 for(unsigned i=0;i<count;++i){if(node==&head || node->_M_prev!=previous){++errors;return;}if(memcmp(&static_cast<Node*>(node)->_M_data,&expected[i],16))++errors;previous=node;node=node->_M_next;}
 if(node!=&head || head._M_prev!=previous)++errors;
}
int main(){unsigned scenarios=0;
 for(unsigned pattern=0;pattern<256;++pattern)for(unsigned direction=0;direction<3;++direction){
  Node head;head._M_next=head._M_prev=&head;Node* storage=&head;List* list=reinterpret_cast<List*>(&storage);Query expected[33],value;
  for(unsigned b=0;b<16;++b)reinterpret_cast<unsigned char*>(&value)[b]=static_cast<unsigned char>(pattern+b*17);
  Node* copied=Access::Create(list,value);if(memcmp(&copied->_M_data,&value,16))++errors;free(copied);
  for(unsigned n=0;n<32;++n){
   const Query* source=&value;if(n && n%3==1)source=&static_cast<Node*>(head._M_next)->_M_data;if(n && n%3==2)source=&static_cast<Node*>(head._M_prev)->_M_data;
   Query before=*source;bool front=direction==0 || (direction==2 && (n&1));
   if(front){memmove(expected+1,expected,n*16);expected[0]=before;list->push_front(*source);}else{expected[n]=before;list->push_back(*source);}
   Check(head,expected,n+1);reinterpret_cast<unsigned char*>(&value)[n%16]^=static_cast<unsigned char>(n+1);
  }
  while(head._M_next!=&head){Node* node=static_cast<Node*>(head._M_next);head._M_next=node->_M_next;free(node);}++scenarios;
 }
 if(errors){printf("TIMER_QUERY_NODES_FAIL errors=%u\n",errors);return 1;}printf("TIMER_QUERY_NODES_BEHAVIOR_PASS scenarios=%u insertions=%u\n",scenarios,scenarios*32);return 0;
}
