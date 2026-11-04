#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_nui_manager.h"
namespace _STL {template<class T,class A> list<T,A>::~list(){free(_M_node);}}
int main(){typedef _STL::list<NUISystem::CActionParam*> L;union{void* align;unsigned char b[12];}u;memset(u.b,0xa5,12);L* p=new(u.b+4) L(_STL::allocator<NUISystem::CActionParam*>());if(!p->_M_node||p->begin()!=p->end()||p->_M_node->_M_next!=p->_M_node||p->_M_node->_M_prev!=p->_M_node||p->size()!=0)return 1;for(int i=0;i<4;++i)if(u.b[i]!=0xa5||u.b[i+8]!=0xa5)return 2;p->~L();puts("ACTION_LIST_CTOR_PASS");return 0;}
