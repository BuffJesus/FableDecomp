#include "fable_ui_bank_storage.h"
void __fastcall FableUiResizeBankRuntime(FableUiGraphicArray* array,void*,unsigned count,const FableUiBankRuntimeEntry* value)
{
    unsigned begin=reinterpret_cast<unsigned>(array->Begin);
    unsigned end=reinterpret_cast<unsigned>(array->End);
    unsigned length=static_cast<long>(end-begin)/12;
    if(count<length) { array->End=reinterpret_cast<void*>(begin+count*12); return; }
    unsigned added=count-length;
    if(!added) return;
    unsigned spare=static_cast<long>(reinterpret_cast<unsigned>(array->Capacity)-end)/12;
    if(spare>=added)
    {
        FableUiBankRuntimeEntry copy=*value;
        FableUiBankRuntimeEntry* target=reinterpret_cast<FableUiBankRuntimeEntry*>(end);
        for(unsigned i=0;i<added;++i) target[i]=copy;
        array->End=reinterpret_cast<void*>(end+added*12);
        return;
    }
    unsigned capacity=length+(length<added ? added : length);
    FableUiBankRuntimeEntry* data=capacity ? static_cast<FableUiBankRuntimeEntry*>(FableUiAllocateArchiveArray(capacity*12)) : 0;
    // Match the original's allocation/copy/fill/free ordering, including a fill
    // value that aliases existing storage. Old storage remains alive until fill.
    FableUiBankRuntimeEntry* source=static_cast<FableUiBankRuntimeEntry*>(array->Begin);
    for(unsigned j=0;j<length;++j) data[j]=source[j];
    for(unsigned k=0;k<added;++k) if(data+length+k) data[length+k]=*value;
    if(array->Begin) FableUiFreeArchiveArray(array->Begin);
    array->End=reinterpret_cast<void*>(reinterpret_cast<unsigned>(data)+count*12);
    array->Begin=data;
    array->Capacity=reinterpret_cast<void*>(reinterpret_cast<unsigned>(data)+capacity*12);
}
