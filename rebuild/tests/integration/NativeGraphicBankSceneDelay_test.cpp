#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphic_bank_scene_delay_aliases.h"
#include "graphic_bank_scene_delay_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall ChangeScene(void*,void*);
extern "C" void __fastcall SetDelay(void*,void*,unsigned);
extern "C" void __fastcall Increment(void*,void*,unsigned);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+17)=(unsigned)(code+80)-(unsigned)(code+21);*(unsigned*)(code+35)=(unsigned)(code+80)-(unsigned)(code+39);
    typedef void (__fastcall *SceneFn)(void*,void*);typedef void (__fastcall *ValueFn)(void*,void*,unsigned);bool retail=strcmp(argv[3],"retail")==0;SceneFn scene=retail?(SceneFn)code:&ChangeScene;ValueFn delay=retail?(ValueFn)(code+48):&SetDelay;ValueFn increment=retail?(ValueFn)(code+80):&Increment;
    unsigned p[3],cases=0;unsigned char expected[4096];
    while(fread(p,4,3,input)==3){
        if(fread(arena,1,4096,input)!=4096)return 6;memcpy(expected,arena,4096);unsigned char* bank=arena+p[0];unsigned char* want=expected+p[0];
        for(unsigned step=0;step<4;++step){
            if(step==0||step==3){*(unsigned*)(want+0x194)+=*(unsigned*)(want+0x268);*(unsigned*)(want+0x1d0)+=*(unsigned*)(want+0x268);scene(bank,0);}
            if(step==1){*(unsigned*)(want+0x26c)=*(unsigned*)(want+0x19c)=p[1];delay(bank,0,p[1]);}
            if(step==2){*(unsigned*)(want+0x194)+=p[2];increment(bank+0x168,0,p[2]);}
            if(memcmp(arena,expected,4096))abort();fwrite(arena,1,4096,output);
        }
        ++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_GRAPHIC_BANK_SCENE_DELAY_PASS cases=%u\n",cases);return 0;
}
