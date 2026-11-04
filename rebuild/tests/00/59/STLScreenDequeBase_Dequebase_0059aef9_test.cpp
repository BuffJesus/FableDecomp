#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_nui_manager.h"
static int calls;static bool zero=true;static size_t amount;static void* receiver;
namespace _STL {template<class T,class A> void _Deque_base<T,A>::_M_initialize_map(size_t n){++calls;amount=n;receiver=this;for(size_t i=0;i<sizeof(*this);++i)if(((unsigned char*)this)[i])zero=false;memset(this,0x73,sizeof(*this));}template void _Deque_base<NUISystem::CComponent*>::_M_initialize_map(size_t);}
int main(){typedef _STL::_Deque_base<NUISystem::CComponent*> B;size_t ns[]={0,1,127,128,129,1024};for(int k=0;k<6;++k){union{double align;unsigned char b[48];}u;memset(u.b,0xa5,48);B* p=new(u.b+4)B(_STL::allocator<NUISystem::CComponent*>(),ns[k]);if(calls!=k+1||receiver!=p||amount!=ns[k]||!zero)return 1;for(int i=0;i<40;++i)if(u.b[4+i]!=0x73)return 2;for(int j=0;j<4;++j)if(u.b[j]!=0xa5||u.b[j+44]!=0xa5)return 3;}puts("SCREEN_DEQUE_BASE_PASS");return 0;}