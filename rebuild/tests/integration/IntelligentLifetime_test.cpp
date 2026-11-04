#include "fable_intelligent_pointer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
static unsigned allocations,deletions,errors;
static void* expectedDeletes[2];
static unsigned char deletedBytes[2][8];
void* operator new(size_t size) {++allocations;++errors;return malloc(size);}
void operator delete(void* block) {
    if(deletions>=2 || block!=expectedDeletes[deletions]) ++errors;
    else memcpy(deletedBytes[deletions],block,8);
    ++deletions;free(block);
}
struct CBaseIntelligentPointerTestAccess {
    static void Set(CBaseIntelligentPointer* p,CBaseObjectPointer* data) {p->PData=data;}
    static CBaseObjectPointer* Data(CBaseIntelligentPointer* p) {return p->PData;}
};
int main() {
    const unsigned long counts[4]={0,1,2,0xffffffff};unsigned scenarios=0;
    for(unsigned pattern=0;pattern<256;++pattern)
    for(unsigned state=0;state<3;++state)
    for(unsigned count=0;count<4;++count)
    for(unsigned destroy=0;destroy<2;++destroy) {
        unsigned long storage[8];memset(storage,pattern,sizeof(storage));
        void* area=destroy?malloc(8):&storage[2];
        CBaseIntelligentPointer* pointer=new(area) CBaseIntelligentPointer;
        unsigned long vtable=*reinterpret_cast<unsigned long*>(pointer);
        if(!vtable || CBaseIntelligentPointerTestAccess::Data(pointer)) return 1;
        CBaseObjectPointer* block=static_cast<CBaseObjectPointer*>(malloc(8));
        block->object=state==2?reinterpret_cast<CBaseObject*>(0x12345000):0;block->refcount=counts[count];
        CBaseIntelligentPointerTestAccess::Set(pointer,state?block:0);
        bool freeBlock=state==1 && counts[count]==1;
        unsigned expectedCount=0;
        if(freeBlock) expectedDeletes[expectedCount++]=block;
        if(destroy) expectedDeletes[expectedCount++]=pointer;
        allocations=deletions=errors=0;
        if(destroy) delete pointer;
        else pointer->~CBaseIntelligentPointer();
        ++scenarios;
        if(allocations || deletions!=expectedCount || errors) return 2;
        if(!freeBlock && block->refcount!=counts[count]-unsigned(state!=0)) return 3;
        unsigned char objectAfter[8];
        if(destroy) memcpy(objectAfter,deletedBytes[expectedCount-1],8);
        else memcpy(objectAfter,pointer,8);
        if(*reinterpret_cast<unsigned long*>(objectAfter)!=vtable || *reinterpret_cast<unsigned long*>(objectAfter+4)) return 4;
        for(unsigned i=0;i<8;++i) if((destroy || i<2 || i>=4) && storage[i]!=(pattern*0x01010101UL)) return 5;
        if(!freeBlock) free(block);
    }
    printf("INTELLIGENT_LIFETIME_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
