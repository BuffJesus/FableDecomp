#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_simple_memory_pool.h"
#include "fable_memory_allocator.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned news,aligned,mutation,blockVtable,baseVtable;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned a=0,unsigned b=0)
{
    unsigned header[]={kind,a,b};unsigned char copy[0x6000];memcpy(copy,arena,sizeof(copy));
    for(unsigned i=0;i<news;++i)
    {
        unsigned& v=*(unsigned*)(copy+0x800+i*0x20);
        if(v==blockVtable)v=0x129d568;else if(v==baseVtable)v=0x12354d0;
    }
    fwrite(header,4,3,output);fwrite(copy,1,sizeof(copy),output);
}
void* __cdecl operator new(size_t size)
{
    if(!arena)return malloc(size);
    if(size!=24||news>=16){event(99);return 0;}
    event(1,24);return arena+0x800+news++*0x20;
}
void __cdecl operator delete(void* p)
{if(!arena){free(p);return;}event(4,(unsigned)p);}
void* FABLE_FASTCALL CMemoryAllocator::AllocateAligned(unsigned long size,long alignment)
{
    if(size>0x500||alignment!=16||aligned>=16){event(99);return 0;}
    blockVtable=word(0x800+(news-1)*0x20);event(2,size,alignment);
    void* result=arena+0x1000+aligned++*0x500;
    if(mutation==1){word(8)+=16;word(12)=64;}else if(mutation==2)word(0x14)=0;
    return result;
}
void FABLE_FASTCALL CMemoryAllocator::DeallocateAligned(void* p)
{
    event(3,(unsigned)p);
    if(mutation==1)word(8)^=0x80;else if(mutation==2)word(0x14)=0;
}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    CIVCountedPointeeBase base;baseVtable=*(unsigned*)&base;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[5],cases=0;const unsigned patterns[][4]={{0,1,15,16},{16,16,16,16},{1,65,17,128},{128,64,32,16},{17,33,65,129},{256,257,64,0}};
    while(fread(params,4,5,input)==5)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;mutation=params[4];news=aligned=0;blockVtable=0;
        CSimpleMemoryPool* pool=(CSimpleMemoryPool*)arena;
        if(params[0]==0)
        {
            for(unsigned i=0;i<4;++i){event(10,(unsigned)pool->Allocate(patterns[params[3]][i]));event(11,pool->CalculateMemoryAllocatedByPool());}
            pool->FreeAll();event(12);event(11,pool->CalculateMemoryAllocatedByPool());
            event(10,(unsigned)pool->Allocate(32));
            pool->FreeAll();event(12);pool->FreeAll();event(12);
        }
        else
        {
            news=1;
            for(unsigned mode=0;mode<3;++mode)
            {
                CSimpleMemoryPool::CMemoryBlock* block=new (arena+0x800) CSimpleMemoryPool::CMemoryBlock(params[1]);event(13);
                block->OwnsData=params[2]!=0;
                if(mode==0){block->CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock();event(14);}
                else
                {
                    typedef void* (__fastcall *DeleteCall)(void*,void*,unsigned);
                    ((DeleteCall)(*(void***)block)[0])(block,0,mode-1);event(15);
                }
            }
        }
        ++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);arena=0;printf("NATIVE_SIMPLE_POOL_LIFETIME_PASS cases=%u\n",cases);return 0;
}
