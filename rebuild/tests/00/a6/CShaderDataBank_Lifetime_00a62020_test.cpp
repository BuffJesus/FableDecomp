#include "fable_shader_data_bank.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static CShaderDataBank* bank;
static unsigned fill, mode, events;
static void* original;
static void* replacement;
static void* derivedTable;
static void require(bool v) { if (!v) abort(); }
extern "C" void UnusedShaderBankBoundary() { abort(); }
extern "C" CBankFile* __fastcall ShaderBankBaseCtor(CBankFile* p) {
    require(p == bank && events == 0); ++events;
    memset(p, fill ^ 0x5a, sizeof(CBankFile)); return p;
}
extern "C" void __fastcall ShaderBankBaseDtor(CBankFile* p) {
    require(p == bank && events == 2); ++events;
    unsigned char* raw = reinterpret_cast<unsigned char*>(bank);
    for (unsigned i=0x120;i<0x128;++i) require(raw[i]==fill);
    require(*reinterpret_cast<void**>(bank)==derivedTable);
    memset(p,0x37,sizeof(CBankFile));
}
void CShaderDataBank::CloseShaders() {
    require(this==bank && events==1); ++events;
    require(Shaders.begin()==original);
    require(*reinterpret_cast<void**>(this)==derivedTable);
    if(mode) {
        free(original);
        Shaders._M_set(static_cast<CShaderBankEntry**>(replacement),
            static_cast<CShaderBankEntry**>(replacement),
            static_cast<CShaderBankEntry**>(replacement));
    }
}
void CShaderBankStateBlock::GetDefault(CBankStateBlock&) { abort(); }
unsigned long CShaderBankStateBlock::CreateCRC() { abort(); return 0; }
void CShaderBankStateBlock::Set(const CBankStateBlock*) { abort(); }
#pragma comment(linker,"/alternatename:??0CBankFile@@QAE@XZ=@ShaderBankBaseCtor@4")
#pragma comment(linker,"/alternatename:??1CBankFile@@UAE@XZ=@ShaderBankBaseDtor@4")
#pragma comment(linker,"/alternatename:?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?OpenReadWrite@CBankFile@@UAEXABVCCharString@@KK@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?OpenFileReadWrite@CBankFile@@UAEXABVCWideString@@KK@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?Close@CShaderDataBank@@UAEXXZ=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?Update@CBankFile@@UAEXXZ=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?GetDataFormatVersion@CShaderDataBank@@MAEKXZ=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?AdjustUpdateEntries@CBankFile@@MAEKXZ=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?NotifyBankSize@CBankFile@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?NotifyBankOpened@CBankFile@@MAEXXZ=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?NotifyEntryAdded@CShaderDataBank@@MAEXKKABV?$CArray@D@@@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?NotifyEntryUpdated@CBankFile@@MAEXKKABV?$CArray@D@@@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?NotifyEntryRemoved@CShaderDataBank@@MAEXK@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?NotifyPostEntriesLoaded@CBankFile@@MAEXXZ=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?PrepareDataForInclusion@CShaderDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?IsDiskFile@CBankFile@@MAE_NKK@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?GetLastModifiedTime@CBankFile@@MAE?AVCDateAndTime@@ABVCCharString@@@Z=_UnusedShaderBankBoundary")
#pragma comment(linker,"/alternatename:?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z=_UnusedShaderBankBoundary")

int main() {
    unsigned scenarios=0;
    for(fill=0;fill<256;++fill) for(mode=0;mode<3;++mode) for(unsigned allocated=0;allocated<2;++allocated) {
        unsigned long storage[0x128/4]; memset(storage,fill,sizeof(storage));
        bank=reinterpret_cast<CShaderDataBank*>(storage);events=0;
        new(bank) CShaderDataBank;
        require(events==1 && bank->Shaders.begin()==0 && bank->Shaders.end()==0 && bank->Shaders.capacity()==0);
        unsigned char* raw=reinterpret_cast<unsigned char*>(storage);
        for(unsigned i=4;i<0x110;++i) require(raw[i]==(fill^0x5a));
        for(i=0x120;i<0x128;++i) require(raw[i]==fill);
        derivedTable=*reinterpret_cast<void**>(bank);
        require(derivedTable!=0);
        original=allocated?malloc(16):0; replacement=mode==1?malloc(24):0;
        bank->Shaders._M_set(static_cast<CShaderBankEntry**>(original),
            static_cast<CShaderBankEntry**>(original),static_cast<CShaderBankEntry**>(original));
        bank->CShaderDataBank::~CShaderDataBank();require(events==3);
        for(i=0;i<0x110;++i) require(raw[i]==0x37);
        for(i=0x120;i<0x128;++i) require(raw[i]==fill);
        ++scenarios;
    }
    printf("SHADER_BANK_LIFETIME_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}

