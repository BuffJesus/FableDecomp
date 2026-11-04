#include <stdio.h>
#include <string.h>
#include "game_map_unload_aliases.h"
#include "game_map_unload_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall World(void*,void*,long);
extern "C" void __fastcall Map(void*);
extern "C" void __fastcall Loaded(void*);
extern "C" void __fastcall Unloaded(void*);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+18)=(unsigned)(code+112)-(unsigned)(code+22);
    *(unsigned*)(code+74)=(unsigned)(code+112)-(unsigned)(code+78);
    typedef void (__fastcall *WorldCall)(void*,void*,long);typedef void (__fastcall *Call)(void*);
    WorldCall world=argv[3][0]=='r'?(WorldCall)code:World;
    Call map=argv[3][0]=='r'?(Call)(code+64):Map;
    Call loaded=argv[3][0]=='r'?(Call)(code+96):Loaded;
    Call unloaded=argv[3][0]=='r'?(Call)(code+112):Unloaded;
    unsigned p[2],cases=0;
    while(fread(p,4,2,in)==2){
        if(fread(arena,1,4096,in)!=4096)return 5;
        for(unsigned i=0;i<2;++i){
            if(p[0]==0)world(arena,0,p[1]);
            if(p[0]==1)map(arena+0x400+p[1]*0x100);
            if(p[0]==2)loaded(arena+0x800);
            if(p[0]==3)unloaded(arena+0x800);
            fwrite(arena,1,4096,out);
        }
        ++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_GAME_MAP_UNLOAD_PASS cases=%u\n",cases);return 0;
}
