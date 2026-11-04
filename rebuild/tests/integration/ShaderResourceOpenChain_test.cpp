
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_shader_resource.h"
#include "fable_game_text_bank.h"
typedef _STL::vector<CShaderBase*> Shaders;
struct CShaderResourceTestAccess {static Shaders& Array(CShaderResource& r){return r.Shaders;} static const CShaderDataBank* Bank(CShaderResource& r){return r.ShaderBank;}};
static unsigned failed;
static void check(bool v){if(!v)++failed;}
int main(){unsigned cases=0;const unsigned uses[]={0,2,5};
 for(unsigned size=0;size<11;++size)for(unsigned valid=0;valid<12;++valid)for(unsigned packed=0;packed<2;++packed)for(unsigned u=0;u<3;++u)for(unsigned spare=0;spare<2;++spare){
  unsigned long ownerWords[15],bankWords[0x110/4];memset(ownerWords,0xa5,sizeof(ownerWords));memset(bankWords,0,sizeof(bankWords));CShaderResource& owner=*reinterpret_cast<CShaderResource*>(ownerWords);CBankFile& base=*reinterpret_cast<CBankFile*>(bankWords);CBankFile::CRuntimeData data[12];memset(data,0,sizeof(data));data[valid].Valid=true;
  unsigned long bits=valid?1UL<<(valid-1):0;base.Size=size;base.RuntimeData._M_start=data;base.PackedValid.Size=packed?size:0;base.PackedValid.PackedInts=&bits;base.PackedValid.Bits=1;base.PackedValid.Bias=0;
  Shaders& array=CShaderResourceTestAccess::Array(owner);unsigned used=uses[u],cap=used+spare*3;CShaderBase** initial=cap?static_cast<CShaderBase**>(malloc(cap*4)):0;
  for(unsigned i=0;i<cap;++i)initial[i]=reinterpret_cast<CShaderBase*>(0x80000000+i*16);array._M_start=initial;array._M_finish=initial+used;array._M_end_of_storage._M_data=initial+cap;
  const CShaderDataBank* bank=reinterpret_cast<const CShaderDataBank*>(&base);owner.OpenShaderBank(bank);unsigned count=valid && valid<size?valid+1:0;bool grow=count>cap;unsigned extra=count-used;unsigned expectedCap=grow?used+(used>extra?used:extra):cap;
  check(CShaderResourceTestAccess::Bank(owner)==bank && array.size()==count && array.capacity()==expectedCap && ((array._M_start!=initial)==grow));
  for(i=0;i<count;++i)check(!array[i]);if(!grow)for(i=count;i<cap;++i)check(array[i]==reinterpret_cast<CShaderBase*>(0x80000000+i*16));
  check(ownerWords[0]==0xa5a5a5a5);for(i=5;i<15;++i)check(ownerWords[i]==0xa5a5a5a5);check(base.Size==size);free(array._M_start);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("SHADER_RESOURCE_OPEN_CHAIN_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
