#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef FABLE_FRAGMENTATION
#include <float.h>
static unsigned long __fastcall nativeFree(CMemoryAllocatorVariableSize* owner,void*){return owner->CMemoryAllocatorVariableSize::GetNoFreeAreas();}
static unsigned long __fastcall nativeAllocated(CMemoryAllocatorVariableSize* owner,void*){return owner->CMemoryAllocatorVariableSize::GetNoAllocatedAreas();}
static unsigned char normalized[0x8000];
#endif
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char expected[0x8000];
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
static bool stateMatches(unsigned char* region)
{
#ifdef FABLE_FRAGMENTATION
    unsigned long* table=reinterpret_cast<unsigned long*>(0x200b000);
    if(table[8]!=reinterpret_cast<unsigned long>(&nativeFree)||table[9]!=reinterpret_cast<unsigned long>(&nativeAllocated))return false;
    memcpy(normalized,region,sizeof(normalized));
    *reinterpret_cast<unsigned long*>(normalized+0x7020)=0xa64530;
    *reinterpret_cast<unsigned long*>(normalized+0x7024)=0xa64560;
    return memcmp(normalized,expected,sizeof(expected))==0;
#else
    return memcmp(region,expected,sizeof(expected))==0;
#endif
}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    unsigned char* region=reinterpret_cast<unsigned char*>(0x2004000);
    CMemoryAllocatorVariableSize* owner=reinterpret_cast<CMemoryAllocatorVariableSize*>(region);
    unsigned long sequences,calls=0,queries=0,largestQueries=0,fragmentationQueries=0,enumerationBulk=0,enumerationIndexed=0,validationQueries=0;read(f,&sequences,4);
    for(unsigned long s=0;s<sequences;++s){
        unsigned long args[2];read(f,args,8);read(f,region,0x8000);
#ifdef FABLE_VALIDATE
        g_FableMemoryAllocator_013CAA90=reinterpret_cast<CMemoryAllocator*>(0x200d800);
#endif
#ifdef FABLE_FRAGMENTATION
        unsigned long* table=reinterpret_cast<unsigned long*>(0x200b000);
        if(*reinterpret_cast<unsigned long*>(region)!=0x200b000||table[8]!=0xa64530||table[9]!=0xa64560)return 10;
        table[8]=reinterpret_cast<unsigned long>(&nativeFree);table[9]=reinterpret_cast<unsigned long>(&nativeAllocated);
#endif
        g_FableVariableAllocatorAlignment_013D28D4=args[0];
        g_FableVariableAllocatorAlignmentShift_013D28D8=args[0]==4?2:args[0]==8?3:args[0]==16?4:5;
        for(unsigned long i=0;i<args[1];++i){
#ifdef FABLE_FREE_CYCLES
            unsigned long step[3];read(f,step,12);read(f,expected,sizeof(expected));++calls;
            unsigned long result=0;
            if(step[0]==0)result=reinterpret_cast<unsigned long>(owner->AllocateMemoryFromExistingChunks(step[1]));
            else reinterpret_cast<CMemoryAllocatorVariableSize::CMemoryAreaChunk*>(0x2008000)->FreeArea(reinterpret_cast<CMemoryAllocatorVariableSize::CMemoryArea*>(step[1]));
            if(result!=step[2]||!stateMatches(region)){printf("sequence %lu operation %lu mismatch\n",s,i);return 5;}
#else
            unsigned long step[2];read(f,step,8);read(f,expected,sizeof(expected));++calls;
            unsigned long result=reinterpret_cast<unsigned long>(owner->AllocateMemoryFromExistingChunks(step[0]));
            if(result!=step[1]||!stateMatches(region)){printf("sequence %lu allocation %lu mismatch\n",s,i);return 5;}
#endif
#ifdef FABLE_SIZE_QUERIES
            unsigned long queryExpected[2];read(f,queryExpected,sizeof(queryExpected));
            if(result){
                void* data=reinterpret_cast<void*>(result+((32+args[0]-1)/args[0])*args[0]);
                unsigned long size=owner->CMemoryAllocatorVariableSize::GetAllocationSize(data);
                unsigned long total=owner->CMemoryAllocatorVariableSize::GetAllocationSizeIncludingHeader(data);queries+=2;
                if(size!=queryExpected[0]||total!=queryExpected[1]||!stateMatches(region))return 8;
            }
#endif
#ifdef FABLE_LARGEST_FREE
            unsigned long expectedLargest;read(f,&expectedLargest,4);
            unsigned long savedAlignment=g_FableVariableAllocatorAlignment_013D28D4,savedShift=g_FableVariableAllocatorAlignmentShift_013D28D8;
            unsigned long largest=owner->CMemoryAllocatorVariableSize::GetLargestFreeAreaSize();++largestQueries;
            if(largest!=expectedLargest||!stateMatches(region)||g_FableVariableAllocatorAlignment_013D28D4!=savedAlignment||g_FableVariableAllocatorAlignmentShift_013D28D8!=savedShift)return 9;
#endif
#ifdef FABLE_FRAGMENTATION
            unsigned long expectedBits;read(f,&expectedBits,4);
            _controlfp(_PC_53|_RC_NEAR|_MCW_EM,_MCW_PC|_MCW_RC|_MCW_EM);_clearfp();
            unsigned long control=_controlfp(0,0);
            float fragmentation=owner->CMemoryAllocatorVariableSize::GetFragmentation();
            unsigned long bits;memcpy(&bits,&fragmentation,4);++fragmentationQueries;
            unsigned long flags=_statusfp();
            if(bits!=expectedBits||flags!=(expectedBits==0x7f800000?_SW_ZERODIVIDE:0)||_controlfp(0,0)!=control||!stateMatches(region)||g_FableVariableAllocatorAlignment_013D28D4!=args[0]||g_FableVariableAllocatorAlignmentShift_013D28D8!=(args[0]==4?2:args[0]==8?3:args[0]==16?4:5))return 11;
#endif
#ifdef FABLE_ENUMERATION
            unsigned long expectedCount,pointers[64];read(f,&expectedCount,4);
            if(expectedCount>=64)return 12;read(f,pointers,expectedCount*4);
            unsigned char* output=reinterpret_cast<unsigned char*>(0x200d000);memset(output,0xcc,256);
            unsigned long count=owner->CMemoryAllocatorVariableSize::GetAllAllocatedAreas(reinterpret_cast<void**>(output),0);++enumerationBulk;
            if(count!=expectedCount||memcmp(output,pointers,count*4)||!stateMatches(region))return 13;
            for(unsigned long tail=count*4;tail<256;++tail)if(output[tail]!=0xcc)return 14;
            for(unsigned long index=0;index<count+4;++index){
                long requested=index<count?static_cast<long>(index):index==count?-1:index==count+1?static_cast<long>(count):index==count+2?static_cast<long>(count+1):0x7fffffff;
                void* pointer=owner->CMemoryAllocatorVariableSize::GetAllocatedArea(requested);++enumerationIndexed;
                if(reinterpret_cast<unsigned long>(pointer)!=(index<count?pointers[index]:0)||!stateMatches(region))return 15;
                if(g_FableVariableAllocatorAlignment_013D28D4!=args[0]||g_FableVariableAllocatorAlignmentShift_013D28D8!=(args[0]==4?2:args[0]==8?3:args[0]==16?4:5))return 16;
            }
#endif
#ifdef FABLE_VALIDATE
            owner->CMemoryAllocatorVariableSize::Validate();++validationQueries;
            if(!stateMatches(region)||g_FableVariableAllocatorAlignment_013D28D4!=args[0]||g_FableVariableAllocatorAlignmentShift_013D28D8!=(args[0]==4?2:args[0]==8?3:args[0]==16?4:5)||g_FableMemoryAllocator_013CAA90!=reinterpret_cast<CMemoryAllocator*>(0x200d800))return 17;
#endif
        }
    }
    if(fgetc(f)!=EOF)return 6;
#ifdef FABLE_SIZE_QUERIES
    printf("VARIABLE_COMPOSED_SIZE_QUERIES_OK queries=%lu\n",queries);
#endif
#ifdef FABLE_LARGEST_FREE
    printf("VARIABLE_COMPOSED_LARGEST_FREE_OK queries=%lu\n",largestQueries);
#endif
#ifdef FABLE_FRAGMENTATION
    printf("VARIABLE_COMPOSED_FRAGMENTATION_OK queries=%lu\n",fragmentationQueries);
#endif
#ifdef FABLE_ENUMERATION
    printf("VARIABLE_COMPOSED_ENUMERATION_OK bulk=%lu indexed=%lu\n",enumerationBulk,enumerationIndexed);
#endif
#ifdef FABLE_VALIDATE
    printf("VARIABLE_COMPOSED_VALIDATE_OK queries=%lu\n",validationQueries);
#endif
#ifdef FABLE_FREE_CYCLES
    printf("VARIABLE_ALLOCATION_FREE_CYCLES_OK sequences=%lu calls=%lu\n",sequences,calls);return 0;
#else
    printf("VARIABLE_EXISTING_ALLOCATIONS_OK sequences=%lu calls=%lu\n",sequences,calls);return 0;
#endif
}
