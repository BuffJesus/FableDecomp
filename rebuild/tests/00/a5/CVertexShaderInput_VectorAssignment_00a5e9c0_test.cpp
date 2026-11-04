
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_vertex_shader_input.h"
typedef CVertexShaderInput::CStreamInfo Stream;
typedef CVertexStreamFormat::CDataElement Element;
typedef _STL::vector<Stream> Streams;
typedef _STL::vector<Element> Elements;
extern "C" void* FormatVtable[];
#pragma comment(linker,"/alternatename:_FormatVtable=??_7CVertexStreamFormat@@6B@")
extern "C" void NeverCalledFormatDelete(){abort();}
#pragma comment(linker,"/alternatename:??_ECVertexStreamFormat@@UAEPAXI@Z=_NeverCalledFormatDelete")
static unsigned failed,copies,assigns,destroys;
static void check(bool v){if(!v)++failed;}
CBaseClass::~CBaseClass(){++destroys;}
namespace _STL {
template<> Elements::vector(const Elements& other):_Vector_base<Element,allocator<Element> >(allocator<Element>()){
 ++copies;size_t n=other.size();_M_start=n?static_cast<Element*>(malloc(n*8)):0;_M_finish=_M_start+n;_M_end_of_storage._M_data=_M_finish;if(n)memcpy(_M_start,other._M_start,n*8);
}
template<> Elements& Elements::operator=(const Elements& other){
 ++assigns;if(this!=&other){size_t n=other.size();Element* p=n?static_cast<Element*>(malloc(n*8)):0;if(n)memcpy(p,other._M_start,n*8);free(_M_start);_M_start=p;_M_finish=p+n;_M_end_of_storage._M_data=_M_finish;}return *this;
}
}
static void initStream(Stream& s,unsigned id,unsigned n){
 s.StreamIndex=id;*reinterpret_cast<unsigned long*>(&s.StreamFormat)=0x12345678;
 Elements& e=s.StreamFormat.DataElements;e._M_start=n?static_cast<Element*>(malloc(n*8)):0;e._M_finish=e._M_start+n;e._M_end_of_storage._M_data=e._M_finish;
 for(unsigned i=0;i<n;++i){e[i].Register=id*10+i;e[i].DataType=static_cast<EVertexStreamDataType>((id+i)%14);}
}
static void dispose(Streams& v){for(Stream* s=v._M_start;s!=v._M_finish;++s)free(s->StreamFormat.DataElements._M_start);free(v._M_start);}
int main(){unsigned cases=0;
 for(unsigned self=0;self<2;++self)for(unsigned used=0;used<4;++used)for(unsigned cap=used;cap<6;++cap)for(unsigned n=0;n<5;++n)for(unsigned nested=0;nested<3;++nested){
  unsigned long destWords[3],sourceWords[3];Streams& dest=*reinterpret_cast<Streams*>(destWords);Streams& source=*reinterpret_cast<Streams*>(sourceWords);
  dest._M_start=cap?static_cast<Stream*>(malloc(cap*20)):0;dest._M_finish=dest._M_start+used;dest._M_end_of_storage._M_data=dest._M_start+cap;
  source._M_start=n?static_cast<Stream*>(malloc(n*20)):0;source._M_finish=source._M_start+n;source._M_end_of_storage._M_data=source._M_finish;
  for(unsigned i=0;i<used;++i)initStream(dest[i],100+i,(i+nested)%3);
  for(i=0;i<n;++i)initStream(source[i],200+i,(i+nested)%3);
  Stream* old=dest._M_start;copies=assigns=destroys=0;Streams& from=self?dest:source;Streams* result=&(dest=from);
  unsigned finalSize=self?used:n;check(result==&dest && dest.size()==finalSize && dest.capacity()==(self||n<=cap?cap:n));
  check(self?dest._M_start==old:((dest._M_start!=old)==(n>cap)));
  check(copies==(self?0:n>cap?n:n>used?n-used:0));check(assigns==(self||n>cap?0:n<used?n:used));check(destroys==(self?0:n>cap?used:used>n?used-n:0));
  for(i=0;i<finalSize;++i){unsigned id=(self?100:200)+i;check(dest[i].StreamIndex==id);Elements& e=dest[i].StreamFormat.DataElements;unsigned nn=(i+nested)%3;check(e.size()==nn);
   for(unsigned j=0;j<nn;++j)check(e[j].Register==id*10+j && e[j].DataType==static_cast<EVertexStreamDataType>((id+j)%14));
   void* expectedVtable=!self && (n>cap || i>=used)?FormatVtable:reinterpret_cast<void*>(0x12345678);check(*reinterpret_cast<void**>(&dest[i].StreamFormat)==expectedVtable);
   if(!self && nn)check(e._M_start!=source[i].StreamFormat.DataElements._M_start);
  }
  for(i=0;i<n;++i)check(source[i].StreamIndex==200+i);
  dispose(dest);dispose(source);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("SHADER_INPUT_VECTOR_ASSIGN_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
