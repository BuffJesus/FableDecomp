#include "fable_ui_bank_storage.h"
static void ClearWords(FableUiGraphicArray* array)
{
    if((static_cast<long>(reinterpret_cast<unsigned>(array->Capacity)-reinterpret_cast<unsigned>(array->Begin))>>2)!=0)
    {
        array->End=array->Begin;
        void* begin=array->Begin; array->Begin=array->End=array->Capacity=0;
        if(begin) FableUiFreeArchiveArray(begin);
    }
}
static void ClearTree(FableUiBankTree* tree)
{
    if(tree->Count)
    {
        FableUiDestroyBankStringTree(tree,0,tree->Head->Parent);
        tree->Head->Left=tree->Head; tree->Head->Parent=0;
        tree->Head->Right=tree->Head; tree->Count=0;
    }
}
void __fastcall FableUiPrepareBankStorage(FableUiBankFileView* bank,void*,unsigned count)
{
    bank->Size=count;
    FableUiClearBankRuntime(&bank->RuntimeData,0);
    FableUiBankRuntimeEntry empty={0,0,0,0,{0,0}};
    FableUiResizeBankRuntime(&bank->RuntimeData,0,count,&empty);
    FableUiClearBankSymbols(&bank->Symbols,0);
    if(bank->OpenFlags&8) FableUiResizeBankSymbols(&bank->Symbols,0,count);
    ClearWords(&bank->Checksums);
    unsigned zero=0;
    if(bank->OpenFlags&0x10) FableUiResizeBankChecksums(&bank->Checksums,0,count,&zero);
    ClearWords(&bank->UpdateData);
    if(bank->OpenFlags&0x20) FableUiResizeBankUpdates(&bank->UpdateData,0,count,&zero);
    ClearTree(&bank->SymbolIndices);
    ClearTree(&bank->FilenameIndices);
}
