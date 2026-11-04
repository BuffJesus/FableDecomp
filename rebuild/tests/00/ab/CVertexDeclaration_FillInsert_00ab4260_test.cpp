#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_shader_input.h"
typedef _STL::vector<D3DVERTEXELEMENT9> Declarations;
static unsigned failed;
static void check(bool value){if(!value)++failed;}
static D3DVERTEXELEMENT9 element(unsigned low,unsigned high){D3DVERTEXELEMENT9 value;unsigned words[2]={low,high};memcpy(&value,words,8);return value;}
int main(){unsigned cases=0;const unsigned lows[]={0,0x55667788,0xffffffff},highs[]={0,0x11223344,0xffffffff};
 for(unsigned used=0;used<5;++used)for(unsigned cap=used;cap<8;++cap)for(unsigned pos=0;pos<=used;++pos)for(unsigned n=0;n<6;++n)for(unsigned alias=0;alias<2;++alias)for(unsigned v=0;v<3;++v){
  if(alias && !used)continue;
  unsigned long words[3];Declarations& array=*reinterpret_cast<Declarations*>(words);D3DVERTEXELEMENT9* initial=cap?static_cast<D3DVERTEXELEMENT9*>(malloc(cap*8)):0;
  for(unsigned i=0;i<cap;++i)initial[i]=element(i<used?0x80000000+i*16:0xaaaaaaaa,0);
  array._M_start=initial;array._M_finish=initial+used;array._M_end_of_storage._M_data=initial+cap;
  D3DVERTEXELEMENT9 external=element(lows[v],highs[v]);D3DVERTEXELEMENT9& value=alias?initial[used/2]:external;value=external;D3DVERTEXELEMENT9 captured=value;
  D3DVERTEXELEMENT9 expected[16];unsigned k=0;for(i=0;i<pos;++i)expected[k++]=initial[i];for(i=0;i<n;++i)expected[k++]=captured;for(i=pos;i<used;++i)expected[k++]=initial[i];
  array._M_fill_insert(initial+pos,n,value);bool grow=n>cap-used;unsigned expectedCap=grow?used+(used>n?used:n):cap;
  check(array.size()==used+n && array.capacity()==expectedCap && ((array._M_start!=initial)==grow));
  if(used+n)check(memcmp(array._M_start,expected,(used+n)*8)==0);
  D3DVERTEXELEMENT9 untouched=element(0xaaaaaaaa,0);if(!grow)for(i=used+n;i<cap;++i)check(memcmp(&array[i],&untouched,8)==0);
  free(array._M_start);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("DECLARATION_FILL_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
