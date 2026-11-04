
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_shader_input.h"
typedef _STL::vector<CVertexStreamFormat::CDataElement> Declarations;
static unsigned failed;
static void check(bool v){if(!v)++failed;}

static unsigned constructorCases(){unsigned cases=0;
 for(unsigned n=0;n<9;++n)for(unsigned fill=0;fill<3;++fill)for(unsigned sentinel=0;sentinel<2;++sentinel){
  unsigned long sourceWords[3],destinationWords[3];Declarations& source=*reinterpret_cast<Declarations*>(sourceWords);memset(destinationWords,sentinel?0xa5:0,sizeof(destinationWords));
  unsigned cap=n+sentinel*2;source._M_start=cap?static_cast<CVertexStreamFormat::CDataElement*>(malloc(cap*8)):0;source._M_finish=source._M_start+n;source._M_end_of_storage._M_data=source._M_start+cap;
  for(unsigned i=0;i<n*8;++i)reinterpret_cast<unsigned char*>(source._M_start)[i]=static_cast<unsigned char>(0x33+fill+i*17);
  Declarations* dest=new(destinationWords) Declarations(source);check(dest==reinterpret_cast<Declarations*>(destinationWords) && dest->size()==n && dest->capacity()==n);
  if(n){check(dest->_M_start!=source._M_start && memcmp(dest->_M_start,source._M_start,n*8)==0);reinterpret_cast<unsigned char*>(dest->_M_start)[0]^=0xff;check(reinterpret_cast<unsigned char*>(source._M_start)[0]==0x33+fill);}else check(!dest->_M_start);
  free(dest->_M_start);free(source._M_start);++cases;
 }
 return cases;
}

int main(){unsigned cases=0;
 for(unsigned self=0;self<2;++self)for(unsigned used=0;used<5;++used)for(unsigned cap=used;cap<7;++cap)for(unsigned n=0;n<8;++n)for(unsigned fill=0;fill<3;++fill){
  unsigned long destWords[3],sourceWords[3];Declarations& dest=*reinterpret_cast<Declarations*>(destWords);Declarations& source=*reinterpret_cast<Declarations*>(sourceWords);
  dest._M_start=cap?static_cast<CVertexStreamFormat::CDataElement*>(malloc(cap*8)):0;dest._M_finish=dest._M_start+used;dest._M_end_of_storage._M_data=dest._M_start+cap;
  source._M_start=n?static_cast<CVertexStreamFormat::CDataElement*>(malloc(n*8)):0;source._M_finish=source._M_start+n;source._M_end_of_storage._M_data=source._M_finish;
  unsigned char expected[64],original[64];for(unsigned i=0;i<cap*8;++i)reinterpret_cast<unsigned char*>(dest._M_start)[i]=static_cast<unsigned char>(0x11+fill+i*13);
  for(i=0;i<n*8;++i)reinterpret_cast<unsigned char*>(source._M_start)[i]=static_cast<unsigned char>(0x55+fill+i*19);
  unsigned finalSize=self?used:n;if(finalSize)memcpy(expected,self?dest._M_start:source._M_start,finalSize*8);if(cap)memcpy(original,dest._M_start,cap*8);
  CVertexStreamFormat::CDataElement* old=dest._M_start;Declarations* result=&(dest=(self?dest:source));
  check(result==&dest && dest.size()==finalSize && dest.capacity()==(self||n<=cap?cap:n));check(self?dest._M_start==old:((dest._M_start!=old)==(n>cap)));
  if(finalSize)check(memcmp(dest._M_start,expected,finalSize*8)==0);
  if(self||n<=cap)for(i=finalSize*8;i<cap*8;++i)check(reinterpret_cast<unsigned char*>(dest._M_start)[i]==original[i]);
  for(i=0;i<n*8;++i)check(reinterpret_cast<unsigned char*>(source._M_start)[i]==static_cast<unsigned char>(0x55+fill+i*19));
  free(dest._M_start);free(source._M_start);++cases;
 }
 cases+=constructorCases();
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("VERTEX_STREAM_ELEMENTS_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
