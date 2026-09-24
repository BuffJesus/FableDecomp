#pragma once
#include "fable_ui_bank_ownership.h"

struct FableUiGraphicsBank;
typedef void (__fastcall *FableUiGraphicsBankDelete)(FableUiGraphicsBank*,void*,unsigned);
typedef void (__fastcall *FableUiGraphicsBankOpen)(FableUiGraphicsBank*,void*,const FableUiStringValue*,unsigned);
struct FableUiGraphicsBankVtable
{
    FableUiGraphicsBankDelete Delete;
    FableUiGraphicsBankOpen Open;
    // Bank opening also uses +0x0C, +0x20 and +0x2C; typed in bank_open.h.
    void* BankMethods08[10];
};
struct FableUiGraphicsBank { FableUiGraphicsBankVtable* Vtable; };
struct FableUiGraphicsBankNode
{
    FableUiGraphicsBankNode* Next;
    FableUiGraphicsBankNode* Previous;
    FableUiGraphicBankInit Init;
    FableUiStringValue Name;
    FableUiBankReference Bank;
};
struct FableUiGraphicsBankManagerView { void* Vtable; FableUiGraphicsBankNode* Banks; };
FABLE_STATIC_ASSERT(sizeof(FableUiGraphicsBankNode)==64);
FABLE_STATIC_ASSERT(offsetof(FableUiGraphicsBankNode,Name)==0x34);
FABLE_STATIC_ASSERT(offsetof(FableUiGraphicsBankNode,Bank)==0x38);
extern unsigned char FableUiGraphicsBankOpenMode; // 013CA7B0: nonzero -> flags zero, otherwise 0xD8.
void* __cdecl FableUiAllocateGraphicsBank(unsigned);
void* __cdecl FableUiAllocateGraphicsBankNode(unsigned);
FableUiGraphicsBank* __fastcall FableUiConstructGraphicsBank(void*,void*); // 009FEA20, 0x30C bytes
void __fastcall FableUiInitialiseGraphicsBank(FableUiGraphicsBank*,void*,const FableUiGraphicBankInit*); // 009FD4E0
void __fastcall FableUiDestroyGraphicsBank(void*); // 00419036
int __fastcall FableUiCompareStringBytes(const char*,const char*); // 00411570, signed chars

struct FableUiProgressView;
typedef void (__fastcall *FableUiProgressStart)(FableUiProgressView*,void*,const FableUiStringValue*,float,bool,bool);
struct FableUiProgressVtable { void* Unrecovered00[3]; FableUiProgressStart Start; };
struct FableUiProgressView { FableUiProgressVtable* Vtable; };
extern FableUiProgressView* FableUiProgressDisplay; // 013CAA38
void __fastcall FableUiStartBankProgress(const FableUiStringValue*,bool,float,bool); // 009E9F40
