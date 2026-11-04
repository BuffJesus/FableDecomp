#include "fable_stlport_string.h"
namespace _STL {
 template<class C,class T,class A> basic_string<C,T,A>::basic_string(size_t count,C character,const A& alloc)
 : _String_base<C,A>(alloc) {
  this->_M_allocate_block(count+1);
  C* end=this->_M_start;
  for(size_t i=0;i<count;++i)*end++=character;
  this->_M_finish=end;
  _M_terminate_string();
 }
 template basic_string<wchar_t>::basic_string(size_t count,wchar_t character,const allocator<wchar_t>& alloc);
}
