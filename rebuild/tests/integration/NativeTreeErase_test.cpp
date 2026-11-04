#include <stdio.h>
#include <string.h>
#include "tree_erase_aliases.h"
#include "tree_erase_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __cdecl EraseEntry(void*,void**,void**,void**);
extern "C" void* __cdecl MinimumEntry(void*);
extern "C" void* __cdecl MaximumEntry(void*);
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<8;++i){unsigned at=retailFixups[i][0],target=retailFixups[i][1];*(unsigned*)(code+at)=(unsigned)(code+target)-(unsigned)(code+at+4);}
    typedef void* (__cdecl *Erase)(void*,void**,void**,void**);typedef void* (__cdecl *Extreme)(void*);
    Erase erase=argv[3][0]=='r'?(Erase)code:EraseEntry;Extreme minimum=argv[3][0]=='r'?(Extreme)(code+640):MinimumEntry;Extreme maximum=argv[3][0]=='r'?(Extreme)(code+672):MaximumEntry;
    unsigned p[2],order[31],cases=0,snapshots=0;
    while(fread(p,4,2,in)==2){
        if(p[1]>31||fread(order,4,31,in)!=31||fread(arena,1,4096,in)!=4096)return 5;
        unsigned char* header=arena+p[0];
        for(unsigned i=0;i<p[1];++i){
            void* root=*(void**)(header+4);unsigned result[3];result[1]=(unsigned)minimum(root);result[2]=(unsigned)maximum(root);
            result[0]=(unsigned)erase(arena+0x500+32*order[i],(void**)(header+4),(void**)(header+8),(void**)(header+12));
            fwrite(result,4,3,out);fwrite(arena,1,4096,out);++snapshots;
        }++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_TREE_ERASE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
