#include "fable_stlport_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include <setjmp.h>
static jmp_buf target;static unsigned throws;
template<> void _STL::_String_base<wchar_t,_STL::allocator<wchar_t> >::_M_throw_length_error() const {++throws;longjmp(target,1);}
int main(){
 typedef _STL::basic_string<wchar_t> String;
 unsigned counts[5]={0,1,2,5,13};wchar_t values[3]={0,0x80,0xffff};
 for(unsigned n=0;n<5;++n)for(unsigned k=0;k<3;++k){
  unsigned char raw[sizeof(String)+8];memset(raw,0xA5,sizeof(raw));String* s=new(raw+4)String(counts[n],values[k]);
  if(s->size()!=counts[n] || s->capacity()!=counts[n] || !s->_M_start || s->_M_start[counts[n]]!=0)return 1;
  for(unsigned i=0;i<counts[n];++i)if(s->_M_start[i]!=values[k])return 2;
  if(raw[3]!=0xA5 || raw[sizeof(raw)-4]!=0xA5)return 3;
  free(s->_M_start);
 }
 unsigned char failed[sizeof(String)];
 if(!setjmp(target)){new(failed)String(0xffffffff,1);return 4;}
 if(!setjmp(target)){new(failed)String(0x7fffffff,1);return 5;}
 if(throws!=2)return 6;
 puts("WIDE_STRING_FILL_0099ca70_PASS");return 0;
}
