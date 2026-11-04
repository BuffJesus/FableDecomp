
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_shader.h"
#include "fable_stlport_vector.h"
typedef _STL::vector<CShaderBase*> Pointers;
static unsigned failed;
static void check(bool v){if(!v)++failed;}
int main(){unsigned cases=0;const unsigned values[]={0,0x55555555,0xffffffff};
 for(unsigned used=0;used<5;++used)for(unsigned cap=used;cap<8;++cap)for(unsigned pos=0;pos<=used;++pos)for(unsigned n=0;n<6;++n)for(unsigned alias=0;alias<2;++alias)for(unsigned v=0;v<3;++v){
  if(alias && !used)continue;
  unsigned long words[3];Pointers& array=*reinterpret_cast<Pointers*>(words);CShaderBase** initial=cap?static_cast<CShaderBase**>(malloc(cap*4)):0;
  for(unsigned i=0;i<cap;++i)initial[i]=reinterpret_cast<CShaderBase*>(i<used?0x80000000+i*16:0xaaaaaaaa);
  array._M_start=initial;array._M_finish=initial+used;array._M_end_of_storage._M_data=initial+cap;
  CShaderBase* external=reinterpret_cast<CShaderBase*>(values[v]);CShaderBase*& value=alias?initial[used/2]:external;value=reinterpret_cast<CShaderBase*>(values[v]);CShaderBase* captured=value;
  CShaderBase* expected[16];unsigned k=0;for(i=0;i<pos;++i)expected[k++]=initial[i];for(i=0;i<n;++i)expected[k++]=captured;for(i=pos;i<used;++i)expected[k++]=initial[i];
  array._M_fill_insert(initial+pos,n,value);bool grow=n>cap-used;unsigned expectedCap=grow?used+(used>n?used:n):cap;
  check(array.size()==used+n && array.capacity()==expectedCap && ((array._M_start!=initial)==grow));
  for(i=0;i<used+n;++i)check(array[i]==expected[i]);
  if(!grow)for(i=used+n;i<cap;++i)check(array[i]==reinterpret_cast<CShaderBase*>(0xaaaaaaaa));
  free(array._M_start);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("SHADER_POINTER_FILL_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
