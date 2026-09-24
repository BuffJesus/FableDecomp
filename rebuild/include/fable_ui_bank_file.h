#pragma once
#include "fable_ui_bank_runtime.h"
#include "engine/CPackedUIntArray.h"
#include <new>

// CBankFile's generated donor layout is 0x174, retail 009CD480 is 0x110.
// Retail RTTI identifies 009D5F80 as CBankFileAsync (0x164), not CBankFile.
struct FableUiBankTreeNode
{
    unsigned char Colour,Unrecovered01[3];
    FableUiBankTreeNode* Parent;
    FableUiBankTreeNode* Left;
    FableUiBankTreeNode* Right;
};
struct FableUiBankTree { FableUiBankTreeNode* Head; unsigned Count,Unrecovered08; };
struct FableUiChecksumCacheView
{
    CWideStringData* Filename;
    FableUiBankTree Entries;
    unsigned BufferSize;
    bool NeedsSaving;
    unsigned char Unrecovered15[3];
};
struct FableUiBankFileView
{
    void* Vtable;
    unsigned Size;
    FableUiGraphicArray Symbols,Checksums,RuntimeData,UpdateData;
    CPackedUIntArray PackedDataOffset,PackedDataSize,PackedDataType,PackedValid;
    unsigned OpenFlags;
    FableUiBankReference BankFile;
    CWideStringData* Filename;
    FableUiStringValue BankHandle;
    bool RetailMode,ReadOnly,WrittenToFile,Solid;
    unsigned ChangeCount;
    bool FileValid,UpdatingFlag;
    unsigned char Unrecovered96[2];
    unsigned Alignment,BankFormatVersion,HeaderSize,FileInformationSize,FileSize;
    FableUiBankTree SymbolIndices,OldSymbolIndices,FilenameIndices;
    FableUiGraphicArray SymbolCRCIndices;
    unsigned char UnrecoveredDC;
    bool SymbolCRCFlag;
    unsigned char UnrecoveredDE[2];
    FableUiGraphicArray BlockOrder;
    void* ProgressCallback;
    FableUiStringValue HeaderEnumerationType;
    bool WarningFlag;
    unsigned char UnrecoveredF5[3];
    FableUiChecksumCacheView ChecksumCache;
};
struct FableUiBankDisposalNode { FableUiBankDisposalNode* Next; FableUiBankDisposalNode* Previous; FableUiBankReference Entry; };
struct FableUiBankFileAsyncView
{
    FableUiBankFileView Base;
    FableUiBankReference ThreadedFile,LoadingMemoryPool,MemoryFailureHandler;
    FableUiBankTree AsyncDataInstances,ReadsToRestart;
    void* LastRestartingData;
    FableUiBankDisposalNode* DisposalList;
    bool NonCached,DisableAsync;
    unsigned char Unrecovered14A[2];
    unsigned char CriticalSection[24];
};
FABLE_STATIC_ASSERT(sizeof(FableUiBankFileView)==0x110);
FABLE_STATIC_ASSERT(offsetof(FableUiBankFileView,Filename)==0x84);
FABLE_STATIC_ASSERT(offsetof(FableUiBankFileView,SymbolIndices)==0xAC);
FABLE_STATIC_ASSERT(offsetof(FableUiBankFileView,ChecksumCache)==0xF8);
FABLE_STATIC_ASSERT(sizeof(FableUiBankFileAsyncView)==0x164);
FABLE_STATIC_ASSERT(offsetof(FableUiBankFileAsyncView,CriticalSection)==0x14C);
extern void* FableUiBankFileVtable; // 0129B54C
extern void* FableUiBankFileAsyncVtable; // 0129B8D4
void __stdcall FableUiInitialiseBankCriticalSection(void*);
CPackedUIntArray* __fastcall FableUiConstructPackedUIntArray(CPackedUIntArray*,void*); // 00A629C0
FableUiChecksumCacheView* __fastcall FableUiConstructChecksumCache(FableUiChecksumCacheView*,void*,unsigned); // 00A60C90
FableUiBankFileView* __fastcall FableUiConstructBankFile(FableUiBankFileView*,void*); // 009CD480
inline void FableUiConstructBankTree(FableUiBankTree* tree,unsigned allocationSize)
{
    tree->Head=0;
    tree->Head=static_cast<FableUiBankTreeNode*>(FableUiAllocateGraphicsBankNode(allocationSize));
    tree->Count=0;
    tree->Head->Colour=0;
    tree->Head->Parent=0;
    tree->Head->Left=tree->Head;
    tree->Head->Right=tree->Head;
}
