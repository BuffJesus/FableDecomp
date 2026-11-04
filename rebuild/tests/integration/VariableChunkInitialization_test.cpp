#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);

class AllocatorAccess : public CMemoryAllocatorVariableSize {
public: using CMemoryAllocatorVariableSize::NewChunkDemandPaged;
};
class ChunkAccess : public CMemoryAllocatorVariableSize::CMemoryAreaChunk {
public: void InsertTest(CMemoryAllocatorVariableSize::CMemoryArea* area){InsertAreaIntoFreeList(area);}
};
static unsigned char before[0x8000],expectedBefore[0x8000],expectedAfter[0x8000];
static unsigned long calls,event[3];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static void word(unsigned long address,unsigned long value){*reinterpret_cast<unsigned long*>(address)=value;}
void CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea* root,CMemoryArea* area)
{
    ++calls;event[0]=reinterpret_cast<unsigned long>(this);
    event[1]=reinterpret_cast<unsigned long>(root);event[2]=reinterpret_cast<unsigned long>(area);
    memcpy(before,region,sizeof(before));
}
static void read(FILE* input,void* data,size_t size)
{
    if(fread(data,1,size,input)!=size){puts("short oracle");exit(2);}
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;
    void* storage=VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4);
    if(storage!=reinterpret_cast<void*>(0x2000000)){puts("fixed test address unavailable");return 3;}
    FILE* input=fopen(argv[1],"rb");if(!input)return 2;
    unsigned long count;read(input,&count,4);
    for(unsigned long caseIndex=0;caseIndex<count;++caseIndex) {
        unsigned long args[5],expectedEvent[3];read(input,args,sizeof(args));
        read(input,expectedEvent,sizeof(expectedEvent));
        read(input,expectedBefore,sizeof(expectedBefore));read(input,expectedAfter,sizeof(expectedAfter));
        unsigned long mode=args[0],alignment=args[1],size=args[2],areas=args[3],pattern=args[4];
        memset(region,pattern,0x8000);calls=0;memset(before,0,sizeof(before));
        g_FableVariableAllocatorAlignment_013D28D4=alignment;
        g_FableVariableAllocatorAlignmentShift_013D28D8=alignment==4?2:alignment==8?3:alignment==16?4:5;
        word(0x2004000+0xc1c,areas&&mode==0?0x200a000:0);word(0x2004000+0xc34,0x200b0c0);
        if(mode==2) {
            long sizes[4]={-8,0,32,128};long total=0;
            for(unsigned long i=0;i<areas;++i)total+=sizes[i];
            word(0x2009000+0x18,0x2004000);word(0x2009000+4,areas);word(0x2009000+0xc,total);
            word(0x2009000+0x28,areas?0x200b000:0);word(0x2009000+0x2c,areas?0x200b000+(areas-1)*0x40:0);
            word(0x2009000+0x10,areas?sizes[areas-1]:0);word(0x2009000+0x14,areas?-8:0);
            for(i=0;i<areas;++i) {
                unsigned long address=0x200b000+i*0x40;
                word(address,i+1<areas?address+0x40:0);word(address+8,i?address-0x40:0);
                word(address+0x18,sizes[i]);word(address+0x14,0x2009000);
            }
            word(0x2009098,size);word(0x2009094,0x2009000);
        }
        AllocatorAccess* owner=reinterpret_cast<AllocatorAccess*>(0x2004000);
        ChunkAccess* chunk=reinterpret_cast<ChunkAccess*>(0x2009000);
        if(mode==0) {
            if(owner->NewChunkDemandPaged(chunk,size)!=chunk)return 4;
        } else if(mode==1)chunk->Init(reinterpret_cast<void*>(0x2009080),size,owner);
        else chunk->InsertTest(reinterpret_cast<CMemoryAllocatorVariableSize::CMemoryArea*>(0x2009080));
        if(calls!=1 || memcmp(event,expectedEvent,sizeof(event)) ||
           memcmp(before,expectedBefore,sizeof(before)) || memcmp(region,expectedAfter,sizeof(expectedAfter))) {
            printf("case %lu mode %lu mismatch\n",caseIndex,mode);return 5;
        }
    }
    if(fgetc(input)!=EOF)return 6;fclose(input);
    VirtualFree(storage,0,0x8000);
    printf("VARIABLE_CHUNK_INITIALIZATION_OK cases=%lu\n",count);return 0;
}
