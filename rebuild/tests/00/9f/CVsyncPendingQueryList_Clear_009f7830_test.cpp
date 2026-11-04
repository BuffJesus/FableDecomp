#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>
typedef _STL::_List_node<CVsyncCallbackTimer::CPendingQuery> Node;
typedef _STL::list<CVsyncCallbackTimer::CPendingQuery> List;
int main(){unsigned scenarios=0;
 for(unsigned count=0;count<128;++count)for(unsigned pattern=0;pattern<4;++pattern){
  Node head;memset(&head,0x39+pattern,sizeof(head));head._M_next=head._M_prev=&head;
  Node* storage[3];storage[0]=reinterpret_cast<Node*>(0x12345678);storage[1]=&head;storage[2]=reinterpret_cast<Node*>(0x87654321);
  for(unsigned i=0;i<count;++i){Node* node=static_cast<Node*>(malloc(sizeof(Node)));memset(node,0xa5,sizeof(*node));node->_M_prev=head._M_prev;node->_M_next=&head;head._M_prev->_M_next=node;head._M_prev=node;}
  CVsyncCallbackTimer::CPendingQuery expected=head._M_data;
  reinterpret_cast<List*>(&storage[1])->clear();++scenarios;
  if(head._M_next!=&head || head._M_prev!=&head || memcmp(&head._M_data,&expected,sizeof(expected)) || storage[1]!=&head || storage[0]!=reinterpret_cast<Node*>(0x12345678) || storage[2]!=reinterpret_cast<Node*>(0x87654321))return 1;
 }
 printf("TIMER_LIST_CLEAR_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
