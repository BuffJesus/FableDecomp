#include "fable_intelligent_pointer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
static unsigned allocations,deletions,errors;
static void* expectedDelete;
void* operator new(size_t size) {if(size!=8) ++errors; ++allocations;return malloc(size);}
void operator delete(void* block) {if(block!=expectedDelete) ++errors; ++deletions;free(block);}
struct CBaseIntelligentPointerTestAccess {
    static void Set(CBaseIntelligentPointer* p,CBaseObjectPointer* data) {p->PData=data;}
    static CBaseObjectPointer* Data(CBaseIntelligentPointer* p) {return p->PData;}
    static void Clear(CBaseIntelligentPointer* p) {p->ClearData();}
};
int main() {
    const unsigned long counts[4]={0,1,2,0xffffffff};unsigned scenarios=0;
    for(unsigned operation=0;operation<3;++operation)
    for(unsigned oldState=0;oldState<4;++oldState)
    for(unsigned oldCount=0;oldCount<4;++oldCount)
    for(unsigned incoming=0;incoming<3;++incoming)
    for(unsigned shared=0;shared<2;++shared)
    for(unsigned newCount=0;newCount<4;++newCount) {
        unsigned long pointerStorage[6]={0xaabbccdd,0x12345678,0,0x99887766,0x12344321,0};
        CBaseIntelligentPointer* pointer=reinterpret_cast<CBaseIntelligentPointer*>(&pointerStorage[1]);
        unsigned long objectStorage[8]={0x31415926,0x11111111,0,0x27182818,0x16180339,0x22222222,0,0x14142135};
        CBaseObject* a=reinterpret_cast<CBaseObject*>(&objectStorage[1]);CBaseObject* b=reinterpret_cast<CBaseObject*>(&objectStorage[5]);
        CBaseObjectPointer* oldBlock=static_cast<CBaseObjectPointer*>(malloc(8));CBaseObjectPointer* newBlock=static_cast<CBaseObjectPointer*>(malloc(8));
        oldBlock->object=oldState==2?a:(oldState==3?b:0);oldBlock->refcount=counts[oldCount];
        CBaseObject* item=incoming==1?a:(incoming==2?b:0);
        newBlock->object=item;newBlock->refcount=counts[newCount];
        if(item) item->IntelligentPointer=shared?newBlock:0;
        CBaseIntelligentPointerTestAccess::Set(pointer,oldState?oldBlock:0);
        bool same=oldState && item && oldBlock->object==item;
        bool clear=oldState && (operation==0 || (operation==1 && oldBlock->object==0) || (operation==2 && !same));
        bool freed=clear && counts[oldCount]==1 && oldBlock->object==0;
        expectedDelete=freed?oldBlock:0;allocations=deletions=errors=0;
        CBaseObject* returned=0;
        if(operation==0) CBaseIntelligentPointerTestAccess::Clear(pointer);
        else if(operation==1) returned=pointer->GetPItem();
        else pointer->SetPItem(item);
        ++scenarios;
        CBaseObjectPointer* got=CBaseIntelligentPointerTestAccess::Data(pointer);
        bool created=operation==2 && !same && item && !shared;
        CBaseObjectPointer* expectedData=(operation==0 || (operation==1 && (oldState==0 || oldState==1)) || (operation==2 && !item))?0:oldBlock;
        if(operation==2 && !same && item) expectedData=shared?newBlock:got;
        if(got!=expectedData || allocations!=unsigned(created) || deletions!=unsigned(freed) || errors) return 1;
        if(operation==1 && returned!=(oldState==2?a:(oldState==3?b:0))) return 2;
        if(!freed && oldBlock->refcount!=counts[oldCount]-unsigned(clear)) return 3;
        if(newBlock->refcount!=counts[newCount]+unsigned(operation==2 && !same && item && shared)) return 4;
        if(created && (!got || got->object!=item || got->refcount!=1 || item->IntelligentPointer!=got)) return 5;
        if(pointerStorage[0]!=0xaabbccdd || pointerStorage[1]!=0x12345678 || pointerStorage[3]!=0x99887766 || objectStorage[0]!=0x31415926 || objectStorage[3]!=0x27182818 || objectStorage[4]!=0x16180339 || objectStorage[7]!=0x14142135) return 6;
        if(!freed) free(oldBlock);free(newBlock);if(created) free(got);
    }
    printf("INTELLIGENT_POINTER_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
