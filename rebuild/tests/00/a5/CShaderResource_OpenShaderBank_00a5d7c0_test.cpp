
#include <stdio.h>
#include <string.h>
#include "fable_shader_resource.h"
#include "fable_game_text_bank.h"
typedef _STL::vector<CShaderBase*> Shaders;
struct CShaderResourceTestAccess {
 static Shaders& Array(CShaderResource& r){return r.Shaders;}
 static CShaderDataBank*& Bank(CShaderResource& r){return r.ShaderBank;}
};
static CShaderResource* owner;
static const CShaderDataBank* bank;
static CShaderBase* alternate[16];
static long maximum;
static unsigned calls,mode,failed;
static void check(bool v){if(!v)++failed;}
long CBankFile::GetMaxEntryIndex() const {
 check(calls++==0 && this==reinterpret_cast<const CBankFile*>(bank));check(CShaderResourceTestAccess::Bank(*owner)==bank);
 if(mode==1)CShaderResourceTestAccess::Bank(*owner)=reinterpret_cast<CShaderDataBank*>(0x12345678);
 return maximum;
}
namespace _STL {
template<> void Shaders::resize(size_t n,CShaderBase* const& value){
 check(calls++==1 && this==&CShaderResourceTestAccess::Array(*owner) && n==static_cast<unsigned long>(maximum+1) && !value);
 this->_M_start=alternate;this->_M_finish=alternate+(mode==2?0:n);this->_M_end_of_storage._M_data=alternate+16;
 if(mode==3 && !n)this->_M_start=this->_M_finish=this->_M_end_of_storage._M_data=0;
}
}
int main(){unsigned cases=0;const long maxima[]={-1,0,1,4,8};const unsigned fills[]={0,0x55,0xa5};
 for(unsigned used=0;used<3;++used)for(unsigned m=0;m<5;++m)for(mode=0;mode<4;++mode)for(unsigned f=0;f<3;++f){
  unsigned long ownerWords[15],bankWords[0x110/4];memset(ownerWords,fills[f],sizeof(ownerWords));owner=reinterpret_cast<CShaderResource*>(ownerWords);bank=reinterpret_cast<CShaderDataBank*>(bankWords);CShaderBase* initial[12];for(unsigned i=0;i<12;++i)initial[i]=reinterpret_cast<CShaderBase*>(0x22222222);
  Shaders& a=CShaderResourceTestAccess::Array(*owner);a._M_start=initial;a._M_finish=initial+used*3;a._M_end_of_storage._M_data=initial+12;
  for(i=0;i<16;++i)alternate[i]=reinterpret_cast<CShaderBase*>(0x55555555);unsigned long before[15];memcpy(before,ownerWords,sizeof(ownerWords));maximum=maxima[m];calls=0;owner->OpenShaderBank(bank);unsigned n=maximum+1;
  check(calls==2 && CShaderResourceTestAccess::Bank(*owner)==(mode==1?reinterpret_cast<CShaderDataBank*>(0x12345678):bank));
  for(i=0;i<16;++i)check(alternate[i]==(i<n?0:reinterpret_cast<CShaderBase*>(0x55555555)));
  for(i=0;i<12;++i)check(initial[i]==reinterpret_cast<CShaderBase*>(0x22222222));
  check(ownerWords[0]==before[0]);for(i=5;i<15;++i)check(ownerWords[i]==before[i]);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("SHADER_RESOURCE_OPEN_BANK_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
