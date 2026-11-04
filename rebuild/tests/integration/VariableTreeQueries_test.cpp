#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class QueryAccess : public CMemoryAllocatorVariableSize {
public:
    using CMemoryAllocatorVariableSize::GetTreeAreaWithSizeGreaterThanOrEqualTo;
    using CMemoryAllocatorVariableSize::GetLargestTreeAreaSize;
    using CMemoryAllocatorVariableSize::GetSmallestTreeAreaSize;
};
static unsigned char state[0x8000];
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
    unsigned char* region=reinterpret_cast<unsigned char*>(0x2004000);QueryAccess* owner=reinterpret_cast<QueryAccess*>(region);
    unsigned long count,calls=0;read(input,&count,4);
    for(unsigned long caseIndex=0;caseIndex<count;++caseIndex) {
        read(input,state,sizeof(state));memcpy(region,state,sizeof(state));
        unsigned long queries,largest,smallest;read(input,&queries,4);
        for(unsigned long i=0;i<queries;++i) {
            unsigned long request,result;read(input,&request,4);read(input,&result,4);++calls;
            if(reinterpret_cast<unsigned long>(owner->GetTreeAreaWithSizeGreaterThanOrEqualTo(request))!=result){printf("query %lu/%lu mismatch\n",caseIndex,i);return 4;}
        }
        read(input,&largest,4);read(input,&smallest,4);calls+=2;
        if(static_cast<unsigned long>(owner->GetLargestTreeAreaSize())!=largest||static_cast<unsigned long>(owner->GetSmallestTreeAreaSize())!=smallest||memcmp(region,state,sizeof(state))) {
            printf("state %lu mismatch\n",caseIndex);return 5;
        }
    }
    if(fgetc(input)!=EOF)return 6;fclose(input);VirtualFree(storage,0,0x8000);
    printf("VARIABLE_TREE_QUERIES_OK states=%lu calls=%lu\n",count,calls);return 0;
}
