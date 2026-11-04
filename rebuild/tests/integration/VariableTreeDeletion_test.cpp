#include "fable_variable_size_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
class TreeAccess:public CMemoryAllocatorVariableSize {
public:using CMemoryAllocatorVariableSize::RemoveFromTree;
    using CMemoryAllocatorVariableSize::RemoveFromTreeFixup;
};
static unsigned char expected[0x8000];
static void read(FILE* f,void* p,unsigned long n){if(fread(p,1,n,f)!=n)exit(2);}
int main(int argc,char** argv)
{
    if(argc!=2)return 2;FILE* f=fopen(argv[1],"rb");if(!f)return 2;
    if(VirtualAlloc(reinterpret_cast<void*>(0x2000000),0x20000,0x3000,4)!=reinterpret_cast<void*>(0x2000000))return 3;
    unsigned char* region=reinterpret_cast<unsigned char*>(0x2004000);
    TreeAccess* owner=reinterpret_cast<TreeAccess*>(region);
    unsigned long sequences,operations=0;read(f,&sequences,4);
    for(unsigned long s=0;s<sequences;++s){
        unsigned long count;read(f,&count,4);read(f,region,0x8000);
        for(unsigned long i=0;i<count;++i){
            unsigned long area;read(f,&area,4);read(f,expected,sizeof(expected));
            owner->RemoveFromTree(reinterpret_cast<CMemoryAllocatorVariableSize::CMemoryArea*>(area));++operations;
            if(memcmp(region,expected,sizeof(expected))){printf("sequence %lu operation %lu mismatch\n",s,i);return 5;}
        }
    }
    unsigned long guards;read(f,&guards,4);
    for(unsigned long g=0;g<guards;++g){
        unsigned long args[2];read(f,args,8);read(f,region,0x8000);read(f,expected,sizeof(expected));
        owner->RemoveFromTreeFixup(reinterpret_cast<CMemoryAllocatorVariableSize::CMemoryArea*>(args[0]),reinterpret_cast<const CDeletedNodeChildInfo*>(args[1]));
        if(memcmp(region,expected,sizeof(expected))){printf("guard %lu mismatch\n",g);return 7;}
    }
    if(fgetc(f)!=EOF)return 6;
    printf("VARIABLE_TREE_DELETION_OK sequences=%lu operations=%lu guards=%lu\n",sequences,operations,guards);return 0;
}
