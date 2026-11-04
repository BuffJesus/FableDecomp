#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_shader_render_manager.h"
#include "fable_simple_memory_pool.h"
#include "fable_memory_allocator.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned news,aligned,mallocs,oldCount,newCount,blockVtable;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned a=0,unsigned b=0)
{
    unsigned header[]={kind,a,b};unsigned char copy[0xc000];memcpy(copy,arena,sizeof(copy));
    for(unsigned i=0;i<news;++i)
    {
        unsigned& v=*(unsigned*)(copy+0x800+i*0x20);if(v==blockVtable)v=0x129d568;
    }
    unsigned lights=word(0x164);
    if(lights&&lights!=(unsigned)(arena+0x1000))
        for(unsigned i=oldCount;i<newCount;++i)
        {
            unsigned offset=lights-(unsigned)arena+i*56;
            memset(copy+offset,0,48);memset(copy+offset+54,0,2);
        }
    fwrite(header,4,3,output);fwrite(copy,1,sizeof(copy),output);
}
void* __cdecl operator new(size_t size)
{
    if(!arena)return malloc(size);if(size!=24||news>=8){event(99);return 0;}
    event(1,24);return arena+0x800+news++*0x20;
}
void __cdecl operator delete(void* p){if(!arena){free(p);return;}event(4,(unsigned)p);}
void* FABLE_FASTCALL CMemoryAllocator::AllocateAligned(unsigned long size,long alignment)
{
    if(size>0x1000||alignment!=16||aligned>=4){event(99);return 0;}
    blockVtable=word(0x800+(news-1)*0x20);event(2,size,alignment);
    return arena+0x6000+aligned++*0x1000;
}
void FABLE_FASTCALL CMemoryAllocator::DeallocateAligned(void* p){event(3,(unsigned)p);}
extern "C" void* __cdecl FableLightsMalloc(size_t size)
{
    if(size>0x1000||mallocs>=3){event(99);return 0;}
    event(5,(unsigned)size);return arena+0x2000+mallocs++*0x1000;
}
extern "C" void __cdecl FableLightsFree(void* p){event(6,(unsigned)p);}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[3],cases=0;
    while(fread(params,4,3,input)==3)
    {
        if(fread(arena,1,0xc000,input)!=0xc000)return 5;
        oldCount=params[0];newCount=params[1];news=aligned=mallocs=blockVtable=0;
        ((CShaderRenderManager*)arena)->AllocateLights(params[1],params[2]);event(10);++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);arena=0;printf("NATIVE_SHADER_LIGHTS_PASS cases=%u\n",cases);return 0;
}
