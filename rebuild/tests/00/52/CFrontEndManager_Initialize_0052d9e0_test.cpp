#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_nui_manager.h"
namespace NUISystem {CObserver::~CObserver(){}}
#define CHECK(x) do{if(!(x)){printf("FAIL %d\n",__LINE__);return 1;}}while(0)
int main(){union {double align;unsigned char b[28];} u;memset(u.b,0xa5,sizeof(u.b));NUISystem::CObserver* p=new(u.b+4) NUISystem::CObserver;
 _STL::set<NUISystem::EEvent>* s=(_STL::set<NUISystem::EEvent>*)(u.b+8);_STL::_Rb_tree_node_base* h=s->_M_t._M_header._M_data;
 CHECK(sizeof(*p)==20);CHECK(h&&h->_M_color==_STL::_S_rb_tree_red&&!h->_M_parent&&h->_M_left==h&&h->_M_right==h);CHECK(s->_M_t._M_node_count==0);CHECK(u.b[20]==0);
 for(int i=0;i<4;++i)CHECK(u.b[i]==0xa5&&u.b[24+i]==0xa5);for(int j=21;j<24;++j)CHECK(u.b[j]==0xa5);
 free(h);p->~CObserver();puts("OBSERVER_CTOR_PASS");return 0;}
