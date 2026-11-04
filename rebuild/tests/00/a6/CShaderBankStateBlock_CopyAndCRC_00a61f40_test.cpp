#include "fable_shader_bank_state.h"
#include "fable_crc.h"
#include <stdio.h>
#include <string.h>
static int failures,calls,mutation;static CShaderBankStateBlock* block;
static unsigned char observed[8];
static void check(bool b){if(!b)++failures;}
void CShaderBankStateBlock::GetDefault(CBankStateBlock&) {} // External boundary; not under test.
class Probe : public CShaderBankStateBlock {
public:static void Copy(CShaderBankStateBlock* to,const CBankStateBlock* from){static_cast<Probe*>(to)->CShaderBankStateBlock::Set(from);}
};
static unsigned hash(const unsigned char* bytes){unsigned result=0x81234567;for(unsigned i=0;i<8;++i)result=(result<<5 | result>>27)^bytes[i];return result;}
fable_u32 CCRC::Calc(const void* data,fable_u32 length) const {
 ++calls;check(this==reinterpret_cast<const CCRC*>(block) && data==&block->State && length==8);memcpy(observed,data,8);unsigned result=hash(observed);
 if(mutation)reinterpret_cast<unsigned char*>(block)[11]^=0x81;return result;
}
int main(){unsigned scenarios=0;const unsigned long targets[]={0,1,0x80000000,0xffffffff};
 for(unsigned fill=0;fill<256;++fill)for(unsigned t=0;t<4;++t)for(int alias=0;alias<2;++alias)for(mutation=0;mutation<2;++mutation){
  unsigned long input[3],output[3],expected[3];memset(input,fill^0x5a,sizeof(input));memset(output,fill,sizeof(output));input[1]=targets[t];output[1]=targets[3-t];
  block=reinterpret_cast<CShaderBankStateBlock*>(output);const CBankStateBlock* from=reinterpret_cast<CBankStateBlock*>(alias?output:input);memcpy(expected,output,sizeof(output));
  const unsigned char* bytes=reinterpret_cast<const unsigned char*>(from);memcpy(reinterpret_cast<unsigned char*>(expected)+4,bytes+4,4);reinterpret_cast<unsigned char*>(expected)[8]=bytes[8];
  Probe::Copy(block,from);check(memcmp(output,expected,sizeof(output))==0);calls=0;unsigned want=hash(reinterpret_cast<unsigned char*>(expected)+4);unsigned got=block->CShaderBankStateBlock::CreateCRC();check(got==want && calls==1 && memcmp(observed,reinterpret_cast<unsigned char*>(expected)+4,8)==0);
  if(mutation)reinterpret_cast<unsigned char*>(expected)[11]^=0x81;check(memcmp(output,expected,sizeof(output))==0);++scenarios;
 }
 printf("SHADER_BANK_STATE_BEHAVIOR_%s scenarios=%u failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
