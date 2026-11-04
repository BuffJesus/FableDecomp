#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class AllocatorAccess : public CMemoryAllocatorVariableSize {
public: using CMemoryAllocatorVariableSize::NewChunk;
    void Configure(unsigned long alignment,unsigned long shift){Alignment=alignment;AlignmentShift=shift;}
};
static unsigned char before[0x8000],expectedBefore[0x8000],expectedAfter[0x8000];
static unsigned long args[9],attempts,treeCalls,eventCount;
static unsigned long events[32][7],expectedEvents[32][7];
static unsigned char* const region=reinterpret_cast<unsigned char*>(0x2004000);
static void word(unsigned long address,unsigned long value){*reinterpret_cast<unsigned long*>(address)=value;}
static void record(unsigned long kind,unsigned long count,unsigned long a,unsigned long b,unsigned long c,unsigned long d,unsigned long result)
{
    if(eventCount>=32)abort();unsigned long* e=events[eventCount++];
    e[0]=kind;e[1]=count;e[2]=a;e[3]=b;e[4]=c;e[5]=d;e[6]=result;
}
static void* __stdcall AllocateRecording(void* address,unsigned long size,unsigned long flags,unsigned long protection)
{
    ++attempts;void* result=attempts<=args[4]?0:reinterpret_cast<void*>(0x2009000);
    record(1,4,reinterpret_cast<unsigned long>(address),size,flags,protection,reinterpret_cast<unsigned long>(result));return result;
}
static unsigned long __stdcall QueryRecording(const void* address,void* info,unsigned long length)
{
    if(address!=reinterpret_cast<void*>(0x2009000)||length!=28)abort();
    unsigned long values[7]={0x2009000,0x2009000,4,args[5],0x1000,4,0x20000};memcpy(info,values,28);
    record(2,2,reinterpret_cast<unsigned long>(address),length,0,0,args[6]);return args[6];
}
extern "C" {
    void* (__stdcall* AllocateSlot)(void*,unsigned long,unsigned long,unsigned long)=AllocateRecording;
    unsigned long (__stdcall* QuerySlot)(const void*,void*,unsigned long)=QueryRecording;
}
#ifndef FABLE_REAL_TREE
void CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea* root,CMemoryArea* area)
{
    ++treeCalls;record(3,3,reinterpret_cast<unsigned long>(this),reinterpret_cast<unsigned long>(root),reinterpret_cast<unsigned long>(area),0,0);
    memcpy(before,region,sizeof(before));
}
#endif
static void read(FILE* input,void* data,size_t size)
{
    if(fread(data,1,size,input)!=size){puts("short oracle");exit(2);}
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;
    void* storage=VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4);
    if(storage!=reinterpret_cast<void*>(0x2000000)){puts("fixed test address unavailable");return 3;}
    FILE* input=fopen(argv[1],"rb");if(!input)return 2;unsigned long count;read(input,&count,4);
    for(unsigned long caseIndex=0;caseIndex<count;++caseIndex) {
        unsigned long result,expectedCount,expectedTreeCalls;read(input,args,sizeof(args));
        read(input,&result,4);read(input,&expectedCount,4);read(input,&expectedTreeCalls,4);
        if(expectedCount>32)return 2;read(input,expectedEvents,expectedCount*28);
        read(input,expectedBefore,sizeof(expectedBefore));read(input,expectedAfter,sizeof(expectedAfter));
        memset(region,args[8],0x8000);memset(before,0,sizeof(before));eventCount=attempts=treeCalls=0;
        unsigned long alignment=args[0],shift=alignment==4?2:alignment==8?3:alignment==16?4:5;
        g_FableVariableAllocatorAlignment_013D28D4=alignment;g_FableVariableAllocatorAlignmentShift_013D28D8=shift;
        AllocatorAccess* owner=reinterpret_cast<AllocatorAccess*>(0x2004000);owner->Configure(alignment,shift);
        word(0x2004000+0xc20,args[2]);word(0x2004000+0xc28,args[3]);word(0x2004000+0xc1c,args[7]?0x200a000:0);
        word(0x2004000+0xc34,0x200b000);g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x2005000);
#ifdef FABLE_REAL_TREE
        word(0x2004000+0xc34,args[7]?0x200b000:0);
        if(args[7]) {
            long values[4];unsigned long nodes=args[7]==1?3:4;
            if(args[7]==1){values[0]=128;values[1]=64;values[2]=256;}
            else if(args[7]==2){values[0]=1024;values[1]=512;values[2]=8192;values[3]=2048;}
            else {values[0]=16384;values[1]=8192;values[2]=32768;values[3]=12288;}
            for(unsigned long i=0;i<nodes;++i) {
                unsigned long node=0x200b000+i*0x40;
                word(node+4,0);word(node+0xc,0);word(node+0x10,0);word(node+0x18,values[i]);
                *reinterpret_cast<unsigned char*>(node+0x1c)=args[7]==1&&i?0:1;
            }
            word(0x200b010,0x200b040);word(0x200b004,0x200b080);
            word(0x200b04c,0x200b000);word(0x200b08c,0x200b000);
            if(args[7]>1) {
                unsigned long parent=args[7]==2?0x200b080:0x200b040;
                word(parent+(args[7]==2?0x10:4),0x200b0c0);word(0x200b0cc,parent);
                *reinterpret_cast<unsigned char*>(0x200b0dc)=0;
            }
        }
#endif
        unsigned long actual=reinterpret_cast<unsigned long>(owner->NewChunk(args[1]));
        if(actual!=result||eventCount!=expectedCount||treeCalls!=expectedTreeCalls||
           memcmp(events,expectedEvents,expectedCount*28)||memcmp(before,expectedBefore,sizeof(before))||
           memcmp(region,expectedAfter,sizeof(expectedAfter))) {
            printf("case %lu mismatch result %lu calls %lu\n",caseIndex,actual,eventCount);return 4;
        }
    }
    if(fgetc(input)!=EOF)return 5;fclose(input);VirtualFree(storage,0,0x8000);
#ifdef FABLE_REAL_TREE
    printf("VARIABLE_CHUNK_TREE_COMPOSED_OK cases=%lu\n",count);return 0;
#else
    printf("VARIABLE_NEW_CHUNK_OK cases=%lu\n",count);return 0;
#endif
}
