#pragma once
#include "fable_ui_bank_decode.h"
struct FableUiBankRuntimeEntry { unsigned Offset,Size; unsigned char Type,Valid,Padding[2]; };
FABLE_STATIC_ASSERT(sizeof(FableUiBankRuntimeEntry)==12);
void __fastcall FableUiDestroyStringRange(FableUiStringValue*,FableUiStringValue*); // 00414E00
FableUiStringValue* __fastcall FableUiEraseStringRange(FableUiGraphicArray*,void*,FableUiStringValue*,FableUiStringValue*); // 0043336A
void __fastcall FableUiClearBankSymbols(FableUiGraphicArray*,void*); // 009D3FB0
void __fastcall FableUiClearBankRuntime(FableUiGraphicArray*,void*); // 009D3D90
void __fastcall FableUiPrepareBankStorage(FableUiBankFileView*,void*,unsigned); // 009CEAE0
void __fastcall FableUiResizeBankRuntime(FableUiGraphicArray*,void*,unsigned,const FableUiBankRuntimeEntry*); // 009D4010
void __fastcall FableUiResizeBankSymbols(FableUiGraphicArray*,void*,unsigned); // 0049B760: resize, not reserve
void __fastcall FableUiResizeBankChecksums(FableUiGraphicArray*,void*,unsigned,const unsigned*); // 00464931
void __fastcall FableUiResizeBankUpdates(FableUiGraphicArray*,void*,unsigned,const unsigned*); // 009D3DF0
void __fastcall FableUiDestroyBankStringTree(FableUiBankTree*,void*,FableUiBankTreeNode*); // 00579435
inline void FableUiResizeWordVector(FableUiGraphicArray* array,unsigned count,const unsigned* value)
{
    unsigned begin=reinterpret_cast<unsigned>(array->Begin),end=reinterpret_cast<unsigned>(array->End);
    unsigned length=static_cast<long>(end-begin)>>2;
    if(count<length) { array->End=reinterpret_cast<void*>(begin+count*4); return; }
    unsigned added=count-length;
    if(!added) return;
    unsigned spare=static_cast<long>(reinterpret_cast<unsigned>(array->Capacity)-end)>>2;
    if(spare>=added)
    {
        unsigned copy=*value; unsigned* target=reinterpret_cast<unsigned*>(end);
        for(unsigned i=0;i<added;++i) target[i]=copy;
        array->End=reinterpret_cast<void*>(end+added*4); return;
    }
    unsigned capacity=length+(length<added ? added : length);
    unsigned* data=capacity ? static_cast<unsigned*>(FableUiAllocateArchiveArray(capacity*4)) : 0;
    unsigned* source=static_cast<unsigned*>(array->Begin);
    for(unsigned j=0;j<length;++j) data[j]=source[j];
    for(unsigned k=0;k<added;++k) data[length+k]=*value;
    if(array->Begin) FableUiFreeArchiveArray(array->Begin);
    array->Begin=data; array->End=data+count; array->Capacity=data+capacity;
}
