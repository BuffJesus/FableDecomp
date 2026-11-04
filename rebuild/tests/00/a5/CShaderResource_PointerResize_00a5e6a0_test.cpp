
#include <stdio.h>
#include <string.h>
#include "fable_vertex_shader.h"
#include "fable_stlport_vector.h"
typedef _STL::vector<CShaderBase*> Pointers;
static Pointers* owner;
static CShaderBase** expectedPosition;
static CShaderBase* const* expectedValue;
static CShaderBase* alternate[16];
static unsigned expectedCount,mode,calls,failed;
static void check(bool v){if(!v)++failed;}
namespace _STL {
template<> void Pointers::_M_fill_insert(CShaderBase** position,size_t n,CShaderBase* const& value){
 ++calls;check(this==owner && position==expectedPosition && n==expectedCount && &value==expectedValue);
 if(mode==1){this->_M_start=alternate;this->_M_finish=alternate+1;this->_M_end_of_storage._M_data=alternate+2;const_cast<CShaderBase*&>(value)=reinterpret_cast<CShaderBase*>(0x12345678);}
 else if(n){size_t used=this->size();if(n>this->capacity()-used){this->_M_start=alternate;this->_M_end_of_storage._M_data=alternate+used+n;}this->_M_finish=this->_M_start+used+n;}
}
}
int main(){unsigned cases=0;const unsigned values[]={0,0x55555555,0xffffffff};
 for(unsigned used=0;used<5;++used)for(unsigned cap=used;cap<7;++cap)for(unsigned target=0;target<8;++target)for(unsigned alias=0;alias<2;++alias)for(unsigned v=0;v<3;++v)for(mode=0;mode<2;++mode){
  if(alias && !used)continue;
  unsigned long words[3];Pointers& array=*reinterpret_cast<Pointers*>(words);owner=&array;CShaderBase* data[16];for(unsigned i=0;i<16;++i)data[i]=reinterpret_cast<CShaderBase*>(0x80000000+i*16);
  array._M_start=cap?data:0;array._M_finish=array._M_start+used;array._M_end_of_storage._M_data=array._M_start+cap;
  CShaderBase* external=reinterpret_cast<CShaderBase*>(values[v]);CShaderBase*& value=alias?data[used/2]:external;value=reinterpret_cast<CShaderBase*>(values[v]);CShaderBase* before[16];memcpy(before,data,sizeof(data));
  expectedPosition=array._M_finish;expectedCount=target-used;expectedValue=&value;calls=0;array.resize(target,value);
  check(calls==(target>=used));
  if(target<used){check(array._M_start==(cap?data:0) && array._M_finish==array._M_start+target && array._M_end_of_storage._M_data==array._M_start+cap);check(memcmp(before,data,sizeof(data))==0);}
  else if(mode==1){check(array._M_start==alternate && array._M_finish==alternate+1 && array._M_end_of_storage._M_data==alternate+2);check(value==reinterpret_cast<CShaderBase*>(0x12345678));}
  else {check(array.size()==target);check(array._M_start==(target>cap?alternate:cap?data:0));check(array.capacity()==(target>cap?target:cap));check(memcmp(before,data,sizeof(data))==0);}
  ++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("SHADER_POINTER_RESIZE_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
