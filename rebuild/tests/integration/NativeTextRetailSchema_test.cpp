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
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
#pragma intrinsic(_ReturnAddress)
extern "C" void ColdBoundary(){fprintf(stderr,"TEXT_PAYLOAD_COLD return=%p\n",_ReturnAddress());fflush(stderr);ExitProcess(80);}
static void require(bool ok,int line){if(!ok){fprintf(stderr,"TEXT_PAYLOAD_FAILURE line=%d\n",line);fflush(stderr);ExitProcess(80);}}
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

static void CheckText(const NGameText::CTextEntry& entry,unsigned round){
 char expected[32];sprintf(expected,"native_%u",round);CCharString narrow(expected);CWideString wide=narrow.ToWideString();CHECK(!wcscmp((const wchar_t*)entry.Text,(const wchar_t*)wide));CHECK(!strcmp((const char*)entry.SpeechBank,"SPEECH") && !strcmp((const char*)entry.Narrator,"NARRATOR") && !strcmp((const char*)entry.SoundName,"SOUND") && entry.Tags.size()==round%5);
 for(unsigned i=0;i<entry.Tags.size();++i){sprintf(expected,"TAG_%u",i);CHECK((unsigned long)entry.Tags[i].Pos==0x80000000+round*7+i && !strcmp((const char*)entry.Tags[i].Name,expected));}
}
static void CheckGroup(const NGameText::CGroupEntry& entry,unsigned round){CHECK(entry.Entries.size()==round%17);for(unsigned i=0;i<entry.Entries.size();++i)CHECK((unsigned long)entry.Entries[i]==0xf0000000+round*11+i*3);}
int main(int argc,char** argv){CHECK(argc==2);unsigned cases=0;
 {CCharString filename(argv[1]);CWideString path=filename.ToWideString();CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;manager.OpenRetailBank(path,false);
 {NGameText::CDataBank bank;CCharString name("RESOURCE_TEST");bank.ResourceList.SetMaximumMemory(0x7fffffff);
 for(unsigned cycle=0;cycle<4;++cycle){CHECK(bank.OpenReadOnly(name,8|64|256));CHECK(bank.Size==67 && bank.Entries.size()==67 && bank.TextEntryBuffer.size()==32 && bank.GroupEntryBuffer.size()==32 && bank.Narrators.size()==3 && !bank.Entries[0] && !bank.Entries[65] && !bank.Entries[66]);CHECK(!strcmp((const char*)bank.Narrators[0],"ALPHA") && !strcmp((const char*)bank.Narrators[1],"") && !strcmp((const char*)bank.Narrators[2],"GAMMA"));for(unsigned m=1;m<=64;++m){if(m%2){CHECK(bank.Entries[m]==&bank.TextEntryBuffer[31-(m-1)/2] && bank.GetTextInfo(m)->NarratorIndex==-5-(long)((m-1)/2));}else CHECK(bank.Entries[m]==&bank.GroupEntryBuffer[31-(m-1)/2]);}CIVCountedPointer<const NGameText::CTextEntry> retainedText;CIVCountedPointer<const NGameText::CGroupEntry> retainedGroup;
 for(unsigned index=1;index<=64;++index){unsigned round=(index-1)/2;bank.ResourceList.CurrentFrame+=5;g_NativeTextLoadingPriority_01396314=(EThreadedFilePriority)(1+index%5);
 if(index%2){CHECK(bank.GetPGroupEntry(index,true).IsNull());const NGameText::CDataBankTextEntry* owner=bank.GetTextInfo(index);CHECK(owner && owner->PText.IsNull());
  if(cycle==1 || cycle==2){CHECK(bank.GetPTextEntry(index,false).IsNull());CHECK(!owner->PText.IsNull() && owner->PText->PFileData.Data);if(cycle==1)CHECK(owner->PText->PFileData->WaitUntilFinished());}
  else if(cycle==3)bank.BeginLoadingText(index);
  {CIVCountedPointer<const NGameText::CTextEntry> first=bank.GetPTextEntry(index,cycle!=1);CHECK(!first.IsNull() && first.data_==owner->PText.data_ && first->IVRefCount==2 && !first->PFileData.Data && first->LastUsedFrame==bank.ResourceList.CurrentFrame);CheckText(*first.data_,round);
   bank.ResourceList.CurrentFrame+=3;{CIVCountedPointer<const NGameText::CTextEntry> second=bank.GetPTextEntry(index,false);CHECK(second.data_==first.data_ && first->IVRefCount==3 && second->LastUsedFrame==bank.ResourceList.CurrentFrame);}CHECK(first->IVRefCount==2);if(index==1){retainedText=first;CHECK(first->IVRefCount==3);}
  }
 }
 else{CHECK(bank.GetPTextEntry(index,true).IsNull());const NGameText::CDataBankGroupEntry* owner=bank.GetGroupInfo(index);CHECK(owner && owner->PGroup.IsNull());
  if(cycle==1 || cycle==2){CHECK(bank.GetPGroupEntry(index,false).IsNull());CHECK(!owner->PGroup.IsNull() && owner->PGroup->PFileData.Data);if(cycle==1)CHECK(owner->PGroup->PFileData->WaitUntilFinished());}
  else if(cycle==3)bank.BeginLoadingGroup(index);
  {CIVCountedPointer<const NGameText::CGroupEntry> first=bank.GetPGroupEntry(index,cycle!=1);CHECK(!first.IsNull() && first.data_==owner->PGroup.data_ && first->IVRefCount==2 && !first->PFileData.Data && first->LastUsedFrame==bank.ResourceList.CurrentFrame);CheckGroup(*first.data_,round);
   bank.ResourceList.CurrentFrame+=3;{CIVCountedPointer<const NGameText::CGroupEntry> second=bank.GetPGroupEntry(index,false);CHECK(second.data_==first.data_ && first->IVRefCount==3 && second->LastUsedFrame==bank.ResourceList.CurrentFrame);}CHECK(first->IVRefCount==2);if(index==2){retainedGroup=first;CHECK(first->IVRefCount==3);}
  }
 }
 CHECK(bank.AsyncDataInstances.empty() && g_FableMemoryAllocator_013CAA90->GetNoAllocations()==0);++cases;
 }
 CHECK(bank.ResourceList.ResourceCount==64 && retainedText->IVRefCount==2 && retainedGroup->IVRefCount==2);bank.Close();bank.Close();CHECK(retainedText->IVRefCount==1 && retainedGroup->IVRefCount==1 && bank.ResourceList.ResourceCount==2);CheckText(*retainedText.data_,0);CheckGroup(*retainedGroup.data_,0);CHECK(bank.ResourceList.AllocatedMemory==retainedText->ResourceSize+retainedGroup->ResourceSize);retainedText.Release();CHECK(bank.ResourceList.ResourceCount==1);retainedGroup.Release();CHECK(bank.ResourceList.ResourceCount==0 && bank.ResourceList.AllocatedMemory==0 && bank.AsyncDataInstances.empty());
 }
 }manager.Close();manager.Close();}
 g_CAFileCurrentPath_013BCA10=L"";CHECK(g_CCharStringInstanceCount_013BD800==0 && g_CWideStringInstanceCount_013BCA20==2);CHECK(g_FableMemoryAllocator_013CAA90->GetNoAllocations()==0);FableMemoryAllocatorShutdown_009f2d20();CHECK(!g_FableMemoryAllocator_013CAA90);printf("NATIVE_TEXT_PUBLIC_GETTERS_PASS cases=%u cycles=4 retained_close=8\n",cases);return 0;
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

