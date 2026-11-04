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

extern "C" void __fastcall BuildNativeGroup(const NGameText::CDataBankGroupEntry*,void*);
extern "C" void __fastcall BuildNativeText(const NGameText::CDataBankTextEntry*,void*);
#pragma comment(linker,"/alternatename:@BuildNativeGroup@8=?BuildGroup@CDataBankGroupEntry@NGameText@@ABEXXZ")
#pragma comment(linker,"/alternatename:@BuildNativeText@8=?BuildText@CDataBankTextEntry@NGameText@@ABEXXZ")
class ResourceBank:public CBankFileAsync{
public:virtual unsigned long GetDataFormatVersion(){return 100;}
virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&){}
virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&){}
};
int main(int argc,char** argv){
 CHECK(argc==2);unsigned cases=0,pending=0;
 {CCharString filename(argv[1]);CWideString path=filename.ToWideString();CBankFileManager& manager=g_FableBankFileManager_013CA79C;manager.UseRetailBanks=true;manager.OpenRetailBank(path,false);
 {ResourceBank bank;CCharString name("RESOURCE_TEST");CHECK(bank.OpenReadOnly(name,8|64|256));CResourceBank resources;CResourceList& list=resources.ResourceList;
 for(unsigned index=1;index<=64;++index)for(unsigned retained=0;retained<2;++retained)for(unsigned prewait=0;prewait<2;++prewait)for(unsigned listed=0;listed<2;++listed){
  unsigned round=(index-1)/2;unsigned long size=bank.GetEntryDataSize(index);unsigned char buffer[512];memset(buffer,0x7d,sizeof(buffer));CHECK(size+32<sizeof(buffer));
  {CCountedPointer<CBankFileAsyncEntry> file=bank.GetAsyncEntry(index);CCountedPointer<CBankFileAsyncData> data=file->BeginReadData(0,buffer+16,size,480,THREADED_FILE_PRIORITY_NORMAL);CHECK(data.Data && data.Info->RefCount==1);if(prewait)CHECK(data->WaitUntilFinished());else if(!data->IsFinished())++pending;
   if(index%2){NGameText::CDataBankTextEntry owner;NGameText::CTextEntry entry;entry.Owner=&owner;owner.PText.Reset(&entry);entry.PFileData.ShareData(data.Data,data.Info);CHECK(data.Info->RefCount==2);if(!retained)data.ReleaseData();if(listed)list.AddResource(entry);++list.CurrentFrame;BuildNativeText(&owner,0);CHECK(!entry.PFileData.Data && !entry.PFileData.Info && entry.LastUsedFrame==(listed?list.CurrentFrame:0) && list.AllocatedMemory==(listed?entry.ResourceSize:0));char expected[32];sprintf(expected,"native_%u",round);CCharString narrow(expected);CWideString wide=narrow.ToWideString();CHECK(!wcscmp((const wchar_t*)entry.Text,(const wchar_t*)wide) && !strcmp((const char*)entry.SpeechBank,"SPEECH") && !strcmp((const char*)entry.Narrator,"NARRATOR") && !strcmp((const char*)entry.SoundName,"SOUND") && entry.Tags.size()==round%5);for(unsigned i=0;i<entry.Tags.size();++i){sprintf(expected,"TAG_%u",i);CHECK((unsigned long)entry.Tags[i].Pos==0x80000000+round*7+i && !strcmp((const char*)entry.Tags[i].Name,expected));}++list.CurrentFrame;BuildNativeText(&owner,0);CHECK(entry.LastUsedFrame==(listed?list.CurrentFrame:0));CHECK(entry.UnloadResource() && owner.PText.IsNull() && entry.IVRefCount==1);}
   else{NGameText::CDataBankGroupEntry owner;NGameText::CGroupEntry entry;entry.Owner=&owner;owner.PGroup.Reset(&entry);entry.PFileData.ShareData(data.Data,data.Info);CHECK(data.Info->RefCount==2);if(!retained)data.ReleaseData();if(listed)list.AddResource(entry);++list.CurrentFrame;BuildNativeGroup(&owner,0);CHECK(!entry.PFileData.Data && !entry.PFileData.Info && entry.LastUsedFrame==(listed?list.CurrentFrame:0) && list.AllocatedMemory==(listed?entry.ResourceSize:0) && entry.Entries.size()==round%17);for(unsigned i=0;i<entry.Entries.size();++i)CHECK((unsigned long)entry.Entries[i]==0xf0000000+round*11+i*3);++list.CurrentFrame;BuildNativeGroup(&owner,0);CHECK(entry.LastUsedFrame==(listed?list.CurrentFrame:0));CHECK(entry.UnloadResource() && owner.PGroup.IsNull() && entry.IVRefCount==1);}
   CHECK(list.ResourceCount==0 && list.AllocatedMemory==0 && list.Head.NextResource==&list.Head && list.Head.PrevResource==&list.Head);if(retained)CHECK(data.Info->RefCount==1 && data->IsFinished() && data->GetData()==buffer+16);
  }
  CHECK(bank.AsyncDataInstances.empty());for(unsigned i=0;i<sizeof(buffer);++i)if(i<16 || i>=16+size)CHECK(buffer[i]==0x7d);++cases;
 }
 bank.Close();bank.Close();CHECK(bank.AsyncDataInstances.empty());}manager.Close();manager.Close();}
 g_CAFileCurrentPath_013BCA10=L"";CHECK(g_CCharStringInstanceCount_013BD800==0 && g_CWideStringInstanceCount_013BCA20==2);printf("NATIVE_TEXT_RESOURCE_BUILD_PASS cases=%u text=256 groups=256\n",cases);CHECK(pending>0);fprintf(stderr,"PENDING_READS %u\n",pending);return 0;
}
#include "NativeFileInputStreamColdAliases.h"
#include "NativeBankLifetimeColdAliases.h"
#include "NativeTextMetadataColdAliases.h"
#pragma comment(linker,"/alternatename:_UnusedBankBoundary=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetLastModifiedTime@CDataBank@NGameText@@MAE?AVCDateAndTime@@ABVCCharString@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?UnloadNextResource@CResourceList@@QAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?HandleFailedAllocation@CResourceList@@UAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z=_ColdBoundary")


bool CResourceBank::ResourceAbortingEnabled=false;
