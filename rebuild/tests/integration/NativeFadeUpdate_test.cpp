#include <stdio.h>
#include <float.h>
#include "fade_update_aliases.h"
extern "C" void __fastcall UpdateFade(void*,void*,float);
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
 unsigned p[2],arena[7],cases=0;while(fread(p,4,2,input)==2){if(fread(arena,1,28,input)!=28)return 5;_clear87();_control87(_MCW_EM|_RC_NEAR|(p[1]==0x27f?_PC_53:_PC_64),_MCW_EM|_MCW_RC|_MCW_PC);UpdateFade(arena,0,*(float*)p);fwrite(arena,1,28,output);++cases;}
 fclose(input);fclose(output);printf("NATIVE_FADE_UPDATE_PASS cases=%u\n",cases);return 0;
}
