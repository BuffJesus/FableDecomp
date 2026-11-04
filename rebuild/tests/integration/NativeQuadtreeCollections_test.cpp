#include <stdio.h>
#include <string.h>
#include "quadtree_collections_aliases.h"
#include "quadtree_collections_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall RemoveEntry(void*);
extern "C" void __fastcall UnloadEntry(void*,void*,void*);
extern "C" void __fastcall DetailEntry(void*,void*,bool);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode;
static void record(unsigned kind,void* self,unsigned argument){unsigned h[]={kind,(unsigned)self,argument};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall Release(void* self,void*,long id){record(1,self,(unsigned)id);}
extern "C" void __fastcall Finish(void* self){record(2,self,0);}
extern "C" void* __fastcall DeleteCollection(void* self,void*,unsigned flags){
    record(3,self,flags);unsigned char* next=*(unsigned char**)((unsigned char*)self+4);
    if(mode==1)*(void**)(owner+4)=0;
    if(mode==2)*(void**)(owner+4)=arena+0xd00;
    if(mode==3&&next)*(void**)(next+4)=0;
    memset(self,0xdd,0x3c);return self;
}
extern "C" void* __fastcall DeleteChild(void* self,void*,unsigned flags){record(4,self,flags);memset(self,0xde,0x4c);return self;}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned* tables=(unsigned*)VirtualAlloc((void*)0x25000000,4096,0x3000,4);if(tables!=(void*)0x25000000)return 7;tables[0]=(unsigned)DeleteCollection;tables[1]=(unsigned)DeleteChild;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+128+18)=(unsigned)Release-(unsigned)(code+128+22);
    *(unsigned*)(code+128+37)=(unsigned)code-(unsigned)(code+128+41);
    *(unsigned*)(code+256+19)=(unsigned)(code+128)-(unsigned)(code+256+23);
    *(unsigned*)(code+256+31)=(unsigned)Finish-(unsigned)(code+256+35);
    typedef void (__fastcall *NoArg)(void*);typedef void (__fastcall *Unload)(void*,void*,void*);typedef void (__fastcall *Detail)(void*,void*,bool);
    NoArg remove=argv[3][0]=='r'?(NoArg)code:RemoveEntry;Unload unload=argv[3][0]=='r'?(Unload)(code+128):UnloadEntry;Detail detail=argv[3][0]=='r'?(Detail)(code+256):DetailEntry;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];owner=arena+p[1];
        for(unsigned i=0;i<2;++i){
            if(p[2]==0)remove(owner);
            else if(p[2]==1)unload(owner,0,arena+0x900);
            else detail(owner-12,0,false);
            record(5,0,i);
        }++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_QUADTREE_COLLECTIONS_PASS cases=%u\n",cases);return 0;
}
