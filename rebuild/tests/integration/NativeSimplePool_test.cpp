#include <stdio.h>
#include "fable_simple_memory_pool.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned created,mutation;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned a=0,unsigned b=0)
{unsigned header[]={kind,a,b};fwrite(header,4,3,output);fwrite(arena,1,0x6000,output);}
void CSimpleMemoryPool::CreateNewMemoryBlock(unsigned long size)
{
    if(this!=(void*)arena||size>0x800||created>=3){event(99);return;}
    event(1,size);unsigned area=0x3000+created*0x900,block=0x900+created*0x20;++created;
    word(area)=size;word(area+4)=word(0x14);word(area+8)=97;word(0x14)=(unsigned)(arena+area);
    word(block)=(unsigned)(arena+0x8000);word(block+4)=1;word(block+8)=(unsigned)(arena+area);
    arena[block+12]=1;word(block+16)=size+16;word(block+20)=word(4);word(4)=(unsigned)(arena+block);
    if(mutation){word(8)=0x180;word(12)=0x240;}
}
static void* __fastcall destroy(CSimpleMemoryPool::CMemoryBlock* receiver,void*,unsigned flags)
{
    if((unsigned char*)receiver<arena+0x800||(unsigned char*)receiver>=arena+0xa00||flags!=1)event(99);
    event(2,(unsigned)receiver,flags);
    if(mutation){receiver->NextBlock=0;word(4)=(unsigned)(arena+0x900);word(0x14)=(unsigned)(arena+0x1800);}
    return receiver;
}
int main(int argc,char** argv)
{
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    word(0x8000)=(unsigned)&destroy;unsigned params[3],cases=0;
    while(fread(params,4,3,input)==3)
    {
        if(fread(arena,1,0x6000,input)!=0x6000)return 5;mutation=params[2];created=0;
        CSimpleMemoryPool* pool=(CSimpleMemoryPool*)arena;
        if(params[0]==0){event(10,(unsigned)pool->Allocate(params[1]));event(10,(unsigned)pool->Allocate(16));}
        else if(params[0]==1){pool->FreeAll();event(11);pool->FreeAll();event(11);}
        else if(params[0]==3){pool->SetCapacity(params[1]);event(12);}
        event(13,pool->CalculateMemoryAllocatedByPool());++cases;
    }
    if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
    VirtualFree(arena,0,0x8000);printf("NATIVE_SIMPLE_POOL_PASS cases=%u\n",cases);return 0;
}
