#include <stdio.h>
#include <string.h>
#include "map_close_children_aliases.h"
#include "map_close_children_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Entry(void*,void*,void*);
extern "C" void __fastcall Detail(void*);
extern "C" void __fastcall Landscape(void*);
static unsigned char* arena;
static unsigned char* owner;
static FILE* output;
static unsigned mode;
static void record(unsigned kind,void* self,unsigned arg){unsigned h[]={kind,(unsigned)self,arg};fwrite(h,4,3,output);fwrite(arena,1,4096,output);}
extern "C" void __fastcall Release(void* self,void*,long id){record(1,self,id);if(mode==1)owner[0x48]|=0xf;}
extern "C" void* __fastcall ChildDelete(void* self,void*,unsigned flags){
    record(2,self,flags);memset(self,0x92,0x4c);
    if(mode==2)*(void**)(owner+20)=0;
    if(mode==3 && self!=arena+0x700)*(void**)(owner+20)=arena+0x700;
    return self;
}
extern "C" void* __fastcall CollectionDelete(void* self,void*,unsigned flags){
    record(3,self,flags);memset(self,0x93,8);
    if(mode==4){*(void**)(owner+4)=0;owner[0x48]|=8;}
    if(mode==5)*(void**)(arena+0xc84)=0;
    return self;
}
extern "C" void __fastcall LandscapeClear(void* self){record(4,self,0);memset(self,0x94,0x60);}
int main(int argc,char** argv){
    if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned* table=(unsigned*)VirtualAlloc((void*)0x25000000,4096,0x3000,4);if(table!=(void*)0x25000000)return 7;table[0]=(unsigned)ChildDelete;table[1]=(unsigned)CollectionDelete;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    *(unsigned*)(code+19)=(unsigned)Release-(unsigned)(code+23);*(unsigned*)(code+139)=(unsigned)code-(unsigned)(code+143);*(unsigned*)(code+164)=(unsigned)LandscapeClear-(unsigned)(code+168);
    typedef void (__fastcall *Call)(void*,void*,void*);typedef void (__fastcall *Close)(void*);
    Call entry=argv[3][0]=='r'?(Call)code:Entry;Close detail=argv[3][0]=='r'?(Close)(code+128):Detail;Close land=argv[3][0]=='r'?(Close)(code+160):Landscape;
    unsigned p[3],cases=0;
    while(fread(p,4,3,in)==3){
        if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];owner=arena+0x100;
        for(unsigned i=0;i<2;++i){
            if(p[1]==0)entry(owner,0,p[2]?arena+0xa00:0);
            if(p[1]==1)detail(owner-12);
            if(p[1]==2)land(arena+0x200);
            record(5,0,i);
        }
        ++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_MAP_CLOSE_CHILDREN_PASS cases=%u\n",cases);return 0;
}
