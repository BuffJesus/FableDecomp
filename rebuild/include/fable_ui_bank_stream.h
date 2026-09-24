#pragma once
#include "fable_ui_bank_open.h"
#include "engine/CDataInputStream.h"
struct FableUiStreamFileVtable
{
    void* Unrecovered00[5];
    void (__fastcall *SetPosition)(void*,void*,unsigned);
    void* Unrecovered18;
    unsigned (__fastcall *GetPosition)(void*,void*);
    void* Unrecovered20;
    unsigned (__fastcall *GetLength)(void*,void*);
    bool (__fastcall *CanSeek)(void*,void*);
};
struct FableUiBankStreamVtable
{
    void* Unrecovered00[2];
    unsigned (__fastcall *GetPosition)(CFileDataInputStream*,void*);
    void* Unrecovered0C[5];
    void (__fastcall *GetSource)(CFileDataInputStream*,void*,void**,long*);
    bool (__fastcall *UseBuffer)(CFileDataInputStream*,void*,long);
    void (__fastcall *ReadDirect)(CFileDataInputStream*,void*,void*,long);
};
extern FableUiBankStreamVtable FableUiFileInputStreamVtable; // 0129A728
FABLE_STATIC_ASSERT(offsetof(FableUiBankStreamVtable,GetSource)==0x20);
FABLE_STATIC_ASSERT(offsetof(FableUiBankStreamVtable,ReadDirect)==0x28);
extern void* FableUiDataInputStreamVtable; // 0129A69C
extern void* FableUiDestroyedBaseVtable; // 01231710
inline CDataInputStream* FableUiStreamPrefix(CFileDataInputStream* stream)
{ return reinterpret_cast<CDataInputStream*>(stream); }
inline FableUiStreamFileVtable* FableUiStreamFileMethods(void* file)
{ return *static_cast<FableUiStreamFileVtable**>(file); }
void __fastcall FableUiCloseBankStream(CFileDataInputStream*,void*); // 00994300
void __fastcall FableUiDestroyStreamBase(void*,void*); // 0099A300
unsigned __fastcall FableUiBankStreamPosition(CFileDataInputStream*,void*); // shared getter 00405FA0
void __fastcall FableUiReadBankStreamSlow(CFileDataInputStream*,void*,void*,long); // 00993CA0
void __fastcall FableUiGetBankStreamSource(CFileDataInputStream*,void*,void**,long*); // 00994360
bool __fastcall FableUiBankStreamUseBuffer(CFileDataInputStream*,void*,long); // 009943B0
void __fastcall FableUiReadBankStreamDirect(CFileDataInputStream*,void*,void*,long); // 009943D0
inline void FableUiReadStreamFile(void* file,void* target,long length)
{
    typedef void (__fastcall *Read)(void*,void*,void*,long,bool);
    reinterpret_cast<Read>(FableUiStreamFileMethods(file)->Unrecovered00[3])(file,0,target,length,false);
}
