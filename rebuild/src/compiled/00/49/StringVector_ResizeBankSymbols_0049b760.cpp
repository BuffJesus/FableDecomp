#include "fable_ui_bank_storage.h"
void __fastcall FableUiResizeBankSymbols(FableUiGraphicArray* array,void*,unsigned count)
{
    FableUiStringValue empty;
    FableUiConstructEmptyString(&empty,0);
    FableUiStringValue* begin=static_cast<FableUiStringValue*>(array->Begin);
    unsigned length=static_cast<long>(reinterpret_cast<unsigned>(array->End)-reinterpret_cast<unsigned>(begin))>>2;
    if(count<length) FableUiEraseStringRange(array,0,begin+count,static_cast<FableUiStringValue*>(array->End));
    else if(count>length)
    {
        unsigned added=count-length;
        unsigned spare=static_cast<long>(reinterpret_cast<unsigned>(array->Capacity)-reinterpret_cast<unsigned>(array->End))>>2;
        if(spare>=added)
        {
            FableUiStringValue temporary;
            FableUiCopyString(&temporary,0,&empty);
            FableUiStringValue* end=static_cast<FableUiStringValue*>(array->End);
            for(unsigned i=0;i<added;++i) FableUiCopyString(end+i,0,&temporary);
            array->End=end+added;
            FableUiDestroyBankName(&temporary,0);
        }
        else
        {
            unsigned capacity=length+(length<added ? added : length);
            FableUiStringValue* data=static_cast<FableUiStringValue*>(FableUiAllocateArchiveArray(capacity*4));
            FableUiStringValue* source=static_cast<FableUiStringValue*>(array->Begin);
            for(unsigned j=0;j<length;++j) FableUiCopyString(data+j,0,source+j);
            for(unsigned k=0;k<added;++k) FableUiCopyString(data+length+k,0,&empty);
            FableUiDestroyStringRange(static_cast<FableUiStringValue*>(array->Begin),static_cast<FableUiStringValue*>(array->End));
            if(array->Begin) FableUiFreeArchiveArray(array->Begin);
            array->Begin=data; array->End=data+count; array->Capacity=data+capacity;
        }
    }
    FableUiDestroyBankName(&empty,0);
}
