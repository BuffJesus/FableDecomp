#include "fable_ui_bank_file.h"
#include <stdio.h>
#include <string.h>
void* FableUiBaseVtable=reinterpret_cast<void*>(0x70000100);
void* FableUiBankFileVtable=reinterpret_cast<void*>(0x70000110);
void* FableUiBankFileAsyncVtable=reinterpret_cast<void*>(0x70000111);
fable_i32 g_CWideStringInstanceCount_013BCA20;
fable_i32 g_CCharStringInstanceCount_013BD800;
static unsigned char storage[0x180],heap[8][64];
static unsigned sizes[8],allocations,seed;
static unsigned Normalize(unsigned value)
{
    unsigned start=reinterpret_cast<unsigned>(heap);
    if(value>=start && value<start+sizeof(heap)) return 0x61000000+value-start;
    start=reinterpret_cast<unsigned>(storage);
    if(value>=start && value<start+sizeof(storage)) return 0x60000000+value-start;
    return value;
}
static void DumpBytes(const unsigned char* p,unsigned size)
{ for(unsigned i=0;i<size;i+=4) { unsigned value; memcpy(&value,p+i,4); printf(":%08x",Normalize(value)); } }
void* __cdecl FableUiAllocateGraphicsBankNode(unsigned size)
{
    printf(" A%u:%u",allocations,size);
    // Observe constructor state at each allocation, before the returned block is installed.
    DumpBytes(storage,sizeof(storage));
    sizes[allocations]=size; return heap[allocations++];
}
void __stdcall FableUiInitialiseBankCriticalSection(void* p)
{
    printf(" CS%u",static_cast<unsigned>(static_cast<unsigned char*>(p)-storage));
    DumpBytes(storage,sizeof(storage));
    for(unsigned i=0;i<24;++i) static_cast<unsigned char*>(p)[i]=static_cast<unsigned char>(seed+i*3);
}
int main()
{
    for(unsigned mode=0;mode<4;++mode) for(seed=0;seed<256;++seed)
    {
        for(unsigned i=0;i<sizeof(storage);++i) storage[i]=static_cast<unsigned char>(seed+i*17);
        for(i=0;i<sizeof(heap);++i) reinterpret_cast<unsigned char*>(heap)[i]=static_cast<unsigned char>(seed+i*11);
        allocations=0; g_CWideStringInstanceCount_013BCA20=seed*3; g_CCharStringInstanceCount_013BD800=seed*5;
        void* object=storage+8; printf("TRACE");
        if(mode==0) { if(FableUiConstructBankFile(static_cast<FableUiBankFileView*>(object),0)!=object) return 1; }
        if(mode==1) { if(FableUiConstructBankFileBase(object,0)!=object) return 2; }
        if(mode==2) { if(FableUiConstructChecksumCache(static_cast<FableUiChecksumCacheView*>(object),0,seed*0x01010101u)!=object) return 3; }
        if(mode==3) { if(FableUiConstructPackedUIntArray(static_cast<CPackedUIntArray*>(object),0)!=object) return 4; }
        printf(" DATA"); DumpBytes(storage,sizeof(storage));
        for(i=0;i<allocations;++i) { printf(" H%u:%u",i,sizes[i]); DumpBytes(heap[i],64); }
        printf(" COUNTS%d:%d END\n",g_CWideStringInstanceCount_013BCA20,g_CCharStringInstanceCount_013BD800);
    }
    return 0;
}
