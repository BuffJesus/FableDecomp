#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
#include "fable_data_input_stream.h"
#include "fable_crc.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CWideString g_CAFileCurrentPath_013BCA10;
unsigned long g_NativeTextReadContext_0138DE48=0x12345678;
unsigned long g_NativeTextReadContext_0138E064=0x87654321;
extern "C" void* _ReturnAddress();
#pragma intrinsic(_ReturnAddress)
extern "C" void ColdBoundary(){fprintf(stderr,"TEXT_PAYLOAD_COLD return=%p\n",_ReturnAddress());exit(80);}
static void require(bool ok,int line){if(!ok){fprintf(stderr,"TEXT_PAYLOAD_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
static bool observe;static unsigned observed;
class ObservedMemory:public CMemoryDataInputStream {
public:
 ObservedMemory(const void* p,unsigned long size):CMemoryDataInputStream(p,size){}
 virtual CCharString ReadNullTerminatedString(){CHECK(!observe || (g_NativeTextReadContext_0138DE48==11 && g_NativeTextReadContext_0138E064==11));++observed;return CMemoryDataInputStream::ReadNullTerminatedString();}
 virtual CWideString ReadNullTerminatedWString(){CHECK(!observe || (g_NativeTextReadContext_0138DE48==11 && g_NativeTextReadContext_0138E064==11));++observed;return CMemoryDataInputStream::ReadNullTerminatedWString();}
};
static unsigned long StringCRC(const CCharString& text){return FableCRC_Calc_004014A0(0,(const char*)text,text.GetLength());}

#include "fable_bank_async_entry.h"
#include "fable_counted_pointer_inline.h"


#include "fable_memory_allocator.h"
extern EThreadedFilePriority g_NativeTextLoadingPriority_01396314;
extern "C" void __fastcall BeginNativeGroup(const NGameText::CDataBankGroupEntry*,void*,unsigned long,NGameText::CDataBank&);
extern "C" void __fastcall BeginNativeText(const NGameText::CDataBankTextEntry*,void*,unsigned long,NGameText::CDataBank&);
extern "C" void __fastcall BuildNativeGroup(const NGameText::CDataBankGroupEntry*,void*);
extern "C" void __fastcall BuildNativeText(const NGameText::CDataBankTextEntry*,void*);
#pragma comment(linker,"/alternatename:@BeginNativeGroup@16=?BeginLoading@CDataBankGroupEntry@NGameText@@ABEXKAAVCDataBank@2@@Z")
#pragma comment(linker,"/alternatename:@BeginNativeText@16=?BeginLoading@CDataBankTextEntry@NGameText@@ABEXKAAVCDataBank@2@@Z")
#pragma comment(linker,"/alternatename:@BuildNativeGroup@8=?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ")
#pragma comment(linker,"/alternatename:@BuildNativeText@8=?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ")
int main(int argc,char** argv){CHECK(argc==2);unsigned cases=0;
 {CCharString filename(argv[1]);CWideString path=filename.ToWideString();CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;manager.OpenRetailBank(path,false);
 {NGameText::CDataBank bank;CCharString name("RESOURCE_TEST");CHECK(bank.OpenReadOnly(name,8|64|256));
 for(unsigned cycle=0;cycle<2;++cycle){
 for(unsigned index=1;index<=64;++index){unsigned round=(index-1)/2;unsigned count=bank.ResourceList.ResourceCount;g_NativeTextLoadingPriority_01396314=(EThreadedFilePriority)(1+index%5);
 if(index%2){const NGameText::CDataBankTextEntry* owner=bank.GetTextInfo(index);CHECK(owner && owner->PText.IsNull());BeginNativeText(owner,0,index,bank);NGameText::CTextEntry* entry=owner->PText.data_;CHECK(entry && entry->Owner==owner && entry->IVRefCount==1 && entry->PFileData.Data && entry->PFileData.Info->RefCount==1);CHECK(entry->PFileData->Length==bank.GetEntryDataSize(index) && entry->PFileData->SkipBytes==0 && entry->PFileData->LoadDesc.FilePos==bank.GetEntryDataOffset(index) && entry->PFileData->LoadDesc.Size==bank.GetEntryDataSize(index) && entry->PFileData->LoadDesc.Priority==g_NativeTextLoadingPriority_01396314);CHECK(entry->ResourceList==&bank.ResourceList && bank.ResourceList.ResourceCount==count+1);EThreadedFilePriority originalPriority=g_NativeTextLoadingPriority_01396314;g_NativeTextLoadingPriority_01396314=THREADED_FILE_PRIORITY_BLOCKING;BeginNativeText(owner,0,index,bank);CHECK(owner->PText.data_==entry && bank.ResourceList.ResourceCount==count+1 && entry->PFileData->LoadDesc.Priority==originalPriority);g_NativeTextLoadingPriority_01396314=originalPriority;BuildNativeText(owner,0);CHECK(entry->ResourceSize==entry->Sizeof()+sizeof(CBankFileAsyncData)+bank.Align(bank.GetEntryDataSize(index)));CHECK(!entry->PFileData.Data && !entry->PFileData.Info);char expected[32];sprintf(expected,"native_%u",round);CCharString narrow(expected);CWideString wide=narrow.ToWideString();CHECK(!wcscmp((const wchar_t*)entry->Text,(const wchar_t*)wide) && !strcmp((const char*)entry->SpeechBank,"SPEECH") && !strcmp((const char*)entry->Narrator,"NARRATOR") && !strcmp((const char*)entry->SoundName,"SOUND") && entry->Tags.size()==round%5);for(unsigned i=0;i<entry->Tags.size();++i){sprintf(expected,"TAG_%u",i);CHECK((unsigned long)entry->Tags[i].Pos==0x80000000+round*7+i && !strcmp((const char*)entry->Tags[i].Name,expected));}}
 else{const NGameText::CDataBankGroupEntry* owner=bank.GetGroupInfo(index);CHECK(owner && owner->PGroup.IsNull());BeginNativeGroup(owner,0,index,bank);NGameText::CGroupEntry* entry=owner->PGroup.data_;CHECK(entry && entry->Owner==owner && entry->IVRefCount==1 && entry->PFileData.Data && entry->PFileData.Info->RefCount==1);CHECK(entry->PFileData->Length==bank.GetEntryDataSize(index) && entry->PFileData->SkipBytes==0 && entry->PFileData->LoadDesc.FilePos==bank.GetEntryDataOffset(index) && entry->PFileData->LoadDesc.Size==bank.GetEntryDataSize(index) && entry->PFileData->LoadDesc.Priority==g_NativeTextLoadingPriority_01396314);CHECK(entry->ResourceList==&bank.ResourceList && bank.ResourceList.ResourceCount==count+1);EThreadedFilePriority originalPriority=g_NativeTextLoadingPriority_01396314;g_NativeTextLoadingPriority_01396314=THREADED_FILE_PRIORITY_BLOCKING;BeginNativeGroup(owner,0,index,bank);CHECK(owner->PGroup.data_==entry && bank.ResourceList.ResourceCount==count+1 && entry->PFileData->LoadDesc.Priority==originalPriority);g_NativeTextLoadingPriority_01396314=originalPriority;BuildNativeGroup(owner,0);CHECK(entry->ResourceSize==entry->Sizeof()+sizeof(CBankFileAsyncData)+bank.Align(bank.GetEntryDataSize(index)));CHECK(!entry->PFileData.Data && !entry->PFileData.Info && entry->Entries.size()==round%17);for(unsigned i=0;i<entry->Entries.size();++i)CHECK((unsigned long)entry->Entries[i]==0xf0000000+round*11+i*3);}
 CHECK(bank.AsyncDataInstances.empty());CHECK(g_FableMemoryAllocator_013CAA90 && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==0);++cases;
 }
 CHECK(bank.ResourceList.ResourceCount==64);bank.Close();CHECK(bank.ResourceList.ResourceCount==0 && bank.ResourceList.AllocatedMemory==0 && bank.AsyncDataInstances.empty());if(cycle==0)CHECK(bank.OpenReadOnly(name,8|64|256));
 }
 }manager.Close();manager.Close();}
 g_CAFileCurrentPath_013BCA10=L"";CHECK(g_CCharStringInstanceCount_013BD800==0 && g_CWideStringInstanceCount_013BCA20==2);CHECK(g_FableMemoryAllocator_013CAA90->GetNoAllocations()==0);FableMemoryAllocatorShutdown_009f2d20();CHECK(!g_FableMemoryAllocator_013CAA90);printf("NATIVE_TEXT_BEGIN_LOADING_PASS cases=%u cycles=2\n",cases);return 0;
}
bool CResourceBank::ResourceAbortingEnabled=false;
#include "NativeFileInputStreamColdAliases.h"
#include "NativeBankLifetimeColdAliases.h"
#include "NativeTextMetadataColdAliases.h"
#pragma comment(linker,"/alternatename:_UnusedBankBoundary=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetLastModifiedTime@CDataBank@NGameText@@MAE?AVCDateAndTime@@ABVCCharString@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?UnloadNextResource@CResourceList@@QAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?HandleFailedAllocation@CResourceList@@UAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z=_ColdBoundary")

