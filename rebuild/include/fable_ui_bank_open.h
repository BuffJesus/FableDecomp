#pragma once
#include "fable_ui_bank_file.h"
#include "fable_threaded_file.h"
#include "engine/CFileDataInputStream.h"
#include "fable_ui_wide_strings.h"
struct FableUiRegisteredBankHeader { unsigned Type,EntryCount,Offset,Unrecovered0C,Alignment; };
struct FableUiRegisteredBank
{
    void* Unrecovered00;
    FableUiBankReference DiskFile,ThreadedFile;
    bool NonCached;
    FableUiBankTree Banks;
};
struct FableUiBankOpenVtable
{
    void* Unrecovered00[3];
    bool (__fastcall *OpenPath)(void*,void*,const FableUiWideStringValue*,unsigned);
    void* Unrecovered10[4];
    unsigned (__fastcall *GetType)(void*,void*);
    void* Unrecovered24[2];
    void (__fastcall *FinishRead)(void*,void*);
};
struct FableUiFilePathVtable
{
    void* Unrecovered00[11];
    FableUiWideStringValue* (__fastcall *GetPath)(void*,void*,FableUiWideStringValue*);
};
FABLE_STATIC_ASSERT(offsetof(FableUiRegisteredBank,ThreadedFile)==12);
FABLE_STATIC_ASSERT(offsetof(FableUiBankOpenVtable,FinishRead)==44);
struct FableUiBankRegistryView;
extern FableUiBankRegistryView FableUiBankRegistryState; // fixed receiver 013CA79C
extern void* FableUiThreadedFileVtable; // 0129A158
bool __fastcall FableUiOpenBankReadOnly(FableUiBankFileView*,void*,const FableUiStringValue*,unsigned); // 009D06F0
bool __fastcall FableUiOpenAsyncBankReadOnly(FableUiBankFileAsyncView*,void*,const FableUiStringValue*,unsigned); // 009D56C0
FableUiWideStringValue* __fastcall FableUiGetBankPath(FableUiBankFileView*,void*,FableUiWideStringValue*); // 009CBF10
CThreadedFile* __fastcall FableUiConstructThreadedFile(CThreadedFile*,void*); // 0098DFD0, existing recovery reconnected
FableUiBankReference* __fastcall FableUiAssignThreadedFile(FableUiBankReference*,void*,const FableUiBankReference*); // 009D6FD0
FableUiBankReference* __fastcall FableUiAssignDiskFile(FableUiBankReference*,void*,const FableUiBankReference*); // 009A9BF0
void __fastcall FableUiReleaseThreadedFile(FableUiBankReference*,void*); // 009A9C40
void __fastcall FableUiReleaseDiskFile(FableUiBankReference*,void*); // 009A9BB0
void __fastcall FableUiReleaseRegisteredBank(FableUiBankReference*,void*); // 009A9D60
void __fastcall FableUiResetThreadedFile(FableUiBankReference*,void*,CThreadedFile*); // 009A9C80
void __fastcall FableUiDeleteThreadedFile(void*); // 009A9040
// Recovered archive lookup and wide-string lifetime, plus remaining file services.
bool __fastcall FableUiFindRegisteredBank(void*,void*,const FableUiStringValue*,FableUiRegisteredBankHeader*,FableUiBankReference*); // 009A7F80
FableUiWideStringValue* __fastcall FableUiFindBankPath(void*,void*,FableUiWideStringValue*,const FableUiStringValue*); // 009A7CA0
void __fastcall FableUiDestroyWideString(FableUiWideStringValue*,void*); // 0099B510
// Recovered stream operations and archive entry reader.
CFileDataInputStream* __fastcall FableUiConstructBankStream(CFileDataInputStream*,void*,void*,unsigned); // 00994700
void __fastcall FableUiSeekBankStream(CFileDataInputStream*,void*,unsigned); // 00993BC0
bool __fastcall FableUiReadBankEntries(FableUiBankFileView*,void*,CFileDataInputStream*,unsigned,unsigned); // 009CFBC0
void __fastcall FableUiDestroyBankStream(CFileDataInputStream*,void*); // 00994780
bool __fastcall FableUiOpenThreadedFile(CThreadedFile*,void*,const FableUiWideStringValue*,bool); // 0098E1E0
