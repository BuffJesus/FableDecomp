#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
#include "fable_data_input_stream.h"
#include "fable_crc.h"
#include "fable_bank_async_entry.h"
extern "C" void ColdBoundary();
extern CBankFileManager g_FableBankFileManager_013CA79C;
static void require(bool ok,int line){if(!ok){fprintf(stderr,"RETAIL_TOC_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
extern "C" bool __fastcall RetailReadEntryList(CBankFile*,void*,CFileDataInputStream&,unsigned long,unsigned long);
#pragma comment(linker,"/alternatename:@RetailReadEntryList@20=?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z")
class TocBank:public CBankFileAsync {
public:
 unsigned added,post,opened;unsigned long format;
 TocBank(unsigned long version):added(0),post(0),opened(0),format(version){}
 virtual unsigned long GetDataFormatVersion(){return format;}
 virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&){}
 virtual void NotifyBankOpened(){++opened;}
 virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&){++added;}
 virtual void NotifyPostEntriesLoaded(){++post;}
};
static void CheckShared(const CCharString& bankName,unsigned slot,CBankFileAsyncData& data,unsigned char* bytes,unsigned long relative,unsigned long count){
 CHECK(data.WaitUntilFinished() && data.IsFinished() && data.GetData()==bytes+16 && data.Length==count);
 for(unsigned k=0;k<96;++k)if(k<16 || k>=16+count)CHECK(bytes[k]==0x9b);
 printf("SHARED\t%s\t%u\t1\t%lu\t%lu\t",(const char*)bankName,slot,relative,count);for(unsigned k=0;k<count;++k)printf("%02x",bytes[16+k]);printf("\n");
}
void InspectRetailToc(NBankFileManager::CRetailBank& retail,const NBankFileManager::CContainedBank& info,const CCharString& bankName){
 long chars=g_CCharStringInstanceCount_013BD800,wides=g_CWideStringInstanceCount_013BCA20;
 CHECK(retail.diskFile_.Info && retail.diskFile_.Info->RefCount==1 && retail.threadedFile_.control_->RefCount==1);
 {CCharString missing("~missing_native_retail_bank~");
  {TocBank bad(info.dataVersion_^1);bad.ChangeCount=0x12345678;bad.FileValid=bad.WrittenToFile=bad.Solid=bad.UpdatingFlag=true;
   CHECK(!bad.OpenReadOnly(bankName,8|64|256));CHECK(bad.ReadOnly && bad.RetailMode && bad.OpenFlags==(8|64|256) && !bad.ChangeCount && !bad.FileValid && !bad.WrittenToFile && !bad.Solid && !bad.UpdatingFlag && !bad.Size && !bad.BankFile.Data && !bad.BankFile.Info && bad.Alignment==1 && !bad.added && !bad.post && !bad.opened);}
  {TocBank absent(info.dataVersion_);CHECK(!absent.OpenReadOnly(missing,8|64|256));CHECK(absent.RetailMode && !absent.FileValid && !absent.Size && !absent.BankFile.Info && !absent.added && !absent.post && !absent.opened);}
  {CCountedPointer<NBankFileManager::CRetailBank> owner(new NBankFileManager::CRetailBank);NBankFileManager::CContainedBank found;
   CHECK(g_FableBankFileManager_013CA79C.FindContainedBank(bankName,found,owner));CHECK(owner.Data==&retail && owner.Info->RefCount==2 && found.dataVersion_==info.dataVersion_ && found.entryCount_==info.entryCount_ && found.infoOffset_==info.infoOffset_ && found.infoSize_==info.infoSize_ && found.alignment_==info.alignment_);
   unsigned char snapshot[20];memcpy(snapshot,&found,20);CHECK(!g_FableBankFileManager_013CA79C.FindContainedBank(missing,found,owner));CHECK(owner.Data==&retail && owner.Info->RefCount==2 && !memcmp(snapshot,&found,20));
   CHECK(g_FableBankFileManager_013CA79C.FindContainedBank(bankName,found,owner) && owner.Info->RefCount==2);
   {CCountedPointer<NBankFileManager::CRetailBank> copy(owner);CHECK(copy.Data==&retail && owner.Info->RefCount==3);copy=copy;CHECK(copy.Info==owner.Info && owner.Info->RefCount==3);}CHECK(owner.Info->RefCount==2);
  }
 }
 CHECK(chars==g_CCharStringInstanceCount_013BD800 && wides==g_CWideStringInstanceCount_013BCA20 && retail.diskFile_.Info->RefCount==1);

 {
  TocBank second(info.dataVersion_);CHECK(second.OpenReadOnly(bankName,8|64|256));
  unsigned long size=second.GetEntryDataSize(1),relative=size/4,count=size-relative;if(count>37)count=37;unsigned char buffers[3][96];memset(buffers,0x9b,sizeof(buffers));
  {TocBank first(info.dataVersion_);CHECK(first.OpenReadOnly(bankName,8|64|256));CHECK(retail.diskFile_.Info->RefCount==3 && retail.threadedFile_.control_->RefCount==3);
   CCountedPointer<CBankFileAsyncEntry> entry=first.GetAsyncEntry(1);CCountedPointer<CBankFileAsyncData> data=entry->BeginReadData(relative,buffers[0]+16,count,64,THREADED_FILE_PRIORITY_NORMAL);
   CHECK(data.Data->WaitUntilFinished() && data.Data->IsFinished());first.Close();first.Close();
   CHECK(!first.Size && !first.BankFile.Info && first.AsyncDataInstances.size()==1 && first.ThreadedFile.control_==retail.threadedFile_.control_ && retail.threadedFile_.data_->openFlag_ && retail.threadedFile_.control_->RefCount==3 && retail.diskFile_.Info->RefCount==2);
   CheckShared(bankName,0,*data,buffers[0],relative,count);
   {CCountedPointer<CBankFileAsyncEntry> e=second.GetAsyncEntry(1);CCountedPointer<CBankFileAsyncData> d=e->BeginReadData(relative,buffers[1]+16,count,64,THREADED_FILE_PRIORITY_HIGH);CheckShared(bankName,1,*d,buffers[1],relative,count);}
  }
  CHECK(retail.threadedFile_.control_->RefCount==2 && retail.diskFile_.Info->RefCount==2 && retail.threadedFile_.data_->openFlag_ && second.AsyncDataInstances.empty());
  {CCountedPointer<CBankFileAsyncEntry> e=second.GetAsyncEntry(1);CCountedPointer<CBankFileAsyncData> d=e->BeginReadData(relative,buffers[2]+16,count,64,THREADED_FILE_PRIORITY_LOW);CheckShared(bankName,2,*d,buffers[2],relative,count);}
 }
 CHECK(retail.threadedFile_.control_->RefCount==1 && retail.diskFile_.Info->RefCount==1);
 {TocBank bank(info.dataVersion_);CHECK(bank.OpenReadOnly(bankName,8|64|256));
 CHECK(bank.ReadOnly && bank.RetailMode && bank.OpenFlags==(8|64|256) && !bank.FileSize && bank.BankFile.Info==retail.diskFile_.Info && retail.diskFile_.Info->RefCount==2 && bank.opened==1);
 CHECK(bank.ThreadedFile.data_==retail.threadedFile_.data_ && bank.ThreadedFile.control_==retail.threadedFile_.control_ && bank.ThreadedFile.control_->RefCount==2 && bank.NonCached==retail.useDma_ && !bank.DisableAsync);
 CHECK(retail.diskFile_.Data->GetPosition()==info.infoOffset_+info.infoSize_);
 CHECK(bank.FileValid && bank.Size==info.entryCount_+2 && bank.CountEntries()==info.entryCount_ && bank.added==info.entryCount_ && bank.post==1);
 {
  unsigned long index[3]={1,info.entryCount_/2?info.entryCount_/2:1,info.entryCount_};unsigned long relative[3],count[3];unsigned char buffers[3][96];memset(buffers,0xe3,sizeof(buffers));
  for(unsigned k=0;k<3;++k){unsigned long size=bank.GetEntryDataSize(index[k]);relative[k]=size/3;count[k]=size-relative[k];if(count[k]>37)count[k]=37;}
  {CCountedPointer<CBankFileAsyncEntry> e0=bank.GetAsyncEntry(index[0]),e1=bank.GetAsyncEntry(index[1]),e2=bank.GetAsyncEntry(index[2]);
   CCountedPointer<CBankFileAsyncData> d0=e0->BeginReadData(relative[0],buffers[0]+16,count[0],64,THREADED_FILE_PRIORITY_LOW);
   CCountedPointer<CBankFileAsyncData> d1=e1->BeginReadData(relative[1],buffers[1]+16,count[1],64,THREADED_FILE_PRIORITY_HIGHEST);
   CCountedPointer<CBankFileAsyncData> d2=e2->BeginReadData(relative[2],buffers[2]+16,count[2],64,THREADED_FILE_PRIORITY_BELOW_NORMAL);
   CHECK(bank.AsyncDataInstances.size()==3 && bank.ReadsToRestart.empty());CBankFileAsyncData* pending[3]={d0.Data,d1.Data,d2.Data};
   for(int k=2;k>=0;--k){CHECK(pending[k]->WaitUntilFinished() && pending[k]->IsFinished() && pending[k]->GetData()==buffers[k]+16 && pending[k]->Length==count[k]);for(unsigned n=0;n<96;++n)if(n<16 || n>=16+count[k])CHECK(buffers[k][n]==0xe3);}
   for(unsigned k=0;k<3;++k){printf("BATCH\t%s\t%u\t%lu\t%lu\t%lu\t",(const char*)bankName,k,index[k],relative[k],count[k]);for(unsigned n=0;n<count[k];++n)printf("%02x",buffers[k][16+n]);printf("\n");}
  }
  CHECK(bank.AsyncDataInstances.empty() && bank.ReadsToRestart.empty() && retail.threadedFile_.control_->RefCount==2);
 }
 for(unsigned long index=1;index<bank.Size;++index)if(bank.EntryExists(index)){
  CCharString symbol=bank.GetEntrySymbol(index);CHECK(bank.FindIndexBySymbol(symbol)==index && bank.FindIndexBySymbolCRC(symbol.GetCRC())==index);
  unsigned long size=bank.GetEntryDataSize(index),offset=bank.GetEntryDataOffset(index);unsigned long fileSize=retail.diskFile_.Data->GetLength();CHECK(offset<=fileSize && size<=fileSize-offset);
  unsigned count=size<17?size:17;unsigned char bytes[64];memset(bytes,0xcd,sizeof(bytes));bank.ReadEntryData(index,0,bytes+16,count);
  for(unsigned k=0;k<sizeof(bytes);++k)if(k<16 || k>=16+count)CHECK(bytes[k]==0xcd);
  printf("ENTRY\t%s\t%lu\t%lu\t%lu\t%lu\t%s\t",(const char*)bankName,index,bank.GetEntryDataType(index),size,offset,(const char*)symbol);
  for(unsigned k=0;k<count;++k)printf("%02x",bytes[16+k]);
  fable_u32 crc=0;unsigned long position=0;unsigned char block[4096+32];
  while(position<size){unsigned long n=size-position;if(n>4096)n=4096;memset(block,0x6d,sizeof(block));bank.ReadEntryData(index,position,block+16,n);for(unsigned k=0;k<16;++k)CHECK(block[k]==0x6d && block[16+n+k]==0x6d);crc=FableCRC_Calc_004014A0(crc,block+16,n);position+=n;}
  printf("\t%08x\n",crc);
  if(index==1 || index==info.entryCount_/2 || index==info.entryCount_){
   unsigned long relative=size/2,n=size-relative;if(n>37)n=37;
   unsigned char queued[96];memset(queued,0xb7,sizeof(queued));
   {CCountedPointer<CBankFileAsyncEntry> entry=bank.GetAsyncEntry(index);
    CHECK(entry.Data && entry.Data->PaddedSize==bank.Align(size));
    {CCountedPointer<CBankFileAsyncData> data=entry->BeginReadData(relative,queued+16,n,64,THREADED_FILE_PRIORITY_NORMAL);
     CHECK(data.Data && data.Data->WaitUntilFinished() && data.Data->IsFinished() && data.Data->GetData()==queued+16 && data.Data->Finished && data.Data->Length==n);
     for(unsigned k=0;k<sizeof(queued);++k)if(k<16 || k>=16+n)CHECK(queued[k]==0xb7);
     printf("ASYNC\t%s\t%lu\t%lu\t%lu\t",(const char*)bankName,index,relative,n);for(unsigned k=0;k<n;++k)printf("%02x",queued[16+k]);printf("\n");
    }
   }
   CHECK(bank.AsyncDataInstances.empty() && bank.ReadsToRestart.empty() && retail.threadedFile_.control_->RefCount==2);
  }

 }
 }
 CHECK(retail.threadedFile_.control_->RefCount==1 && retail.threadedFile_.data_->openFlag_ && retail.diskFile_.Info->RefCount==1 && retail.diskFile_.Data->IsOpen() && chars==g_CCharStringInstanceCount_013BD800 && wides==g_CWideStringInstanceCount_013BCA20);
}
#include "NativeBankLifetimeColdAliases.h"
#pragma comment(linker,"/alternatename:_UnusedBankBoundary=_ColdBoundary")
#pragma comment(linker,"/alternatename:??4CCharString@@QAEABV0@PBD@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetSubString@CCharString@@QBE?AV1@KK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?FindCharPos@CCharString@@QBEJD@Z=_ColdBoundary")
