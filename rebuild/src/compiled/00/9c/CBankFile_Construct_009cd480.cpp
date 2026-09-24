#include "fable_ui_bank_file.h"
FableUiBankFileView* __fastcall FableUiConstructBankFile(FableUiBankFileView* bank,void*)
{
    FableUiConstructBase(bank,0);
    bank->Vtable=FableUiBankFileVtable;
    bank->Symbols.Begin=bank->Symbols.End=bank->Symbols.Capacity=0;
    bank->Checksums.Begin=bank->Checksums.End=bank->Checksums.Capacity=0;
    bank->RuntimeData.Begin=bank->RuntimeData.End=bank->RuntimeData.Capacity=0;
    bank->UpdateData.Begin=bank->UpdateData.End=bank->UpdateData.Capacity=0;
    FableUiConstructPackedUIntArray(&bank->PackedDataOffset,0);
    FableUiConstructPackedUIntArray(&bank->PackedDataSize,0);
    FableUiConstructPackedUIntArray(&bank->PackedDataType,0);
    FableUiConstructPackedUIntArray(&bank->PackedValid,0);
    bank->BankFile.Data=0; bank->BankFile.Info=0;
    new (&bank->Filename) CWideString;
    FableUiConstructEmptyString(&bank->BankHandle,0);
    FableUiConstructBankTree(&bank->SymbolIndices,24);
    FableUiConstructBankTree(&bank->OldSymbolIndices,24);
    FableUiConstructBankTree(&bank->FilenameIndices,24);
    bank->SymbolCRCIndices.Begin=bank->SymbolCRCIndices.End=bank->SymbolCRCIndices.Capacity=0;
    bank->SymbolCRCFlag=false;
    bank->BlockOrder.Begin=bank->BlockOrder.End=bank->BlockOrder.Capacity=0;
    FableUiConstructEmptyString(&bank->HeaderEnumerationType,0);
    FableUiConstructChecksumCache(&bank->ChecksumCache,0,0x10000);
    bank->HeaderSize=0; bank->FileInformationSize=0; bank->FileSize=0;
    bank->ProgressCallback=0; bank->Size=0;
    bank->BankFormatVersion=0x73; bank->Alignment=1;
    return bank;
}
