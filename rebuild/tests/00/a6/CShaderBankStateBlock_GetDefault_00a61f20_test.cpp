#include "fable_shader_bank_state.h"
#include <stdio.h>
#include <string.h>
int main(int argc,char** argv){
 if(argc!=2)return 2;FILE* out=fopen(argv[1],"wb");if(!out)return 3;
 const unsigned offsets[]={0,28,32,64};unsigned cases=0;
 for(unsigned fill=0;fill<256;++fill)for(unsigned i=0;i<4;++i){
  unsigned long storage[32];memset(storage,fill,sizeof(storage));unsigned char* raw=(unsigned char*)storage;
  CShaderBankStateBlock* receiver=(CShaderBankStateBlock*)(raw+32);
  receiver->CShaderBankStateBlock::GetDefault(*(CBankStateBlock*)(raw+offsets[i]));
  if(fwrite(raw,1,sizeof(storage),out)!=sizeof(storage))return 4;++cases;
 }
 fclose(out);printf("SHADER_BANK_DEFAULT_PASS cases=%u\n",cases);return 0;
}
