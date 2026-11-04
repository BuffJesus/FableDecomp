#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_bank_file_data_type_count.h"
extern "C" void* _ReturnAddress();
#pragma intrinsic(_ReturnAddress)
extern "C" void ColdBoundary(){fprintf(stderr,"TEXT_BOUNDARY_COLD return=%p\n",_ReturnAddress());exit(80);}
static void require(bool ok,int line){if(!ok){fprintf(stderr,"TEXT_BOUNDARY_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
template<class T> struct View {
 CArray<T> array;
 View(T* data,unsigned count){array._M_start=data;array._M_finish=data+count;array._M_end_of_storage._M_data=data+count;}
 ~View(){array._M_start=array._M_finish=array._M_end_of_storage._M_data=0;}
};
class Probe:public NGameText::CDataBank {
public:
 unsigned long Version(){return GetDataFormatVersion();}
 void SizeNotice(unsigned long size,const CArray<CBankFileDataTypeCount>& counts){NGameText::CDataBank::NotifyBankSize(size,counts);}
 void EntryNotice(unsigned long type,unsigned long index,const CArray<char>& info){NGameText::CDataBank::NotifyEntryAdded(type,index,info);}
};
static unsigned TableInfo(char* bytes,unsigned count,const char* const* names){memcpy(bytes,&count,4);unsigned size=4;for(unsigned i=0;i<count;++i){unsigned n=(unsigned)strlen(names[i])+1;memcpy(bytes+size,names[i],n);size+=n;}return size;}
int main(){
 long chars=g_CCharStringInstanceCount_013BD800,wides=g_CWideStringInstanceCount_013BCA20;unsigned sizeCases=0,entryCases=0;
 for(unsigned test=0;test<512;++test){
  {Probe bank;CHECK(bank.Version()==106);bank.ReadOnly=(test&1)!=0;unsigned count=(test>>1)%8,size=test*37%65;CBankFileDataTypeCount rows[8];unsigned texts=0,groups=0;
   for(unsigned i=0;i<count;++i){rows[i].DataType=(i+test)%5;rows[i].Count=(i*7+test)%13;if(rows[i].DataType==0)texts+=rows[i].Count;else if(rows[i].DataType==1)groups+=rows[i].Count;}
   View<CBankFileDataTypeCount> counts(rows,count);bank.SizeNotice(size,counts.array);
   CHECK(bank.Entries.size()==(bank.ReadOnly?size:0) && bank.TextEntryBuffer.capacity()==(bank.ReadOnly?texts:0) && bank.GroupEntryBuffer.capacity()==(bank.ReadOnly?groups:0) && !bank.TextEntryBuffer.size() && !bank.GroupEntryBuffer.size());
   for(unsigned i=0;i<bank.Entries.size();++i)CHECK(!bank.Entries[i]);++sizeCases;
  }
  {Probe bank;bank.ReadOnly=(test&1)!=0;CBankFileDataTypeCount rows[2];rows[0].DataType=0;rows[0].Count=2;rows[1].DataType=1;rows[1].Count=2;View<CBankFileDataTypeCount> counts(rows,2);bank.SizeNotice(4,counts.array);
   long narrator=-(long)(test%17);View<char> text((char*)&narrator,4);char dummy=0;View<char> empty(&dummy,0);
   bank.EntryNotice(0,1,text.array);bank.EntryNotice(1,3,empty.array);
   const char* names[3]={"ALPHA","BETA","GAMMA"};unsigned n=(test>>2)%4;char bytes[64];unsigned bytesCount=TableInfo(bytes,n,names);{View<char> info(bytes,bytesCount);bank.EntryNotice(2,8,info.array);}
   if(bank.ReadOnly){CHECK(bank.Narrators.size()==n);for(unsigned i=0;i<n;++i)CHECK(!strcmp((const char*)bank.Narrators[i],names[i]));}
   narrator-=37;bank.EntryNotice(0,5,text.array);bank.EntryNotice(1,12,empty.array);bank.EntryNotice(7,20,empty.array);
   if(bank.ReadOnly){
    CHECK(bank.Entries.size()==21 && bank.TextEntryBuffer.size()==2 && bank.GroupEntryBuffer.size()==2);
    CHECK(bank.Entries[1]==&bank.TextEntryBuffer[0] && bank.Entries[5]==&bank.TextEntryBuffer[1] && bank.TextEntryBuffer[0].NarratorIndex==narrator+37 && bank.TextEntryBuffer[1].NarratorIndex==narrator && !bank.TextEntryBuffer[0].PText.data_ && !bank.TextEntryBuffer[1].PText.data_);
    CHECK(bank.Entries[3]==&bank.GroupEntryBuffer[0] && bank.Entries[12]==&bank.GroupEntryBuffer[1] && !bank.GroupEntryBuffer[0].PGroup.data_ && !bank.GroupEntryBuffer[1].PGroup.data_ && !bank.Entries[8] && !bank.Entries[20]);
   }else CHECK(!bank.Entries.size() && !bank.TextEntryBuffer.size() && !bank.GroupEntryBuffer.size() && !bank.Narrators.size());
   const char* finalName="FINAL";bytesCount=TableInfo(bytes,1,&finalName);{View<char> info(bytes,bytesCount);bank.EntryNotice(2,17,info.array);}if(bank.ReadOnly)CHECK(bank.Narrators.size()==1 && !strcmp((const char*)bank.Narrators[0],"FINAL"));
   bytesCount=TableInfo(bytes,0,names);{View<char> info(bytes,bytesCount);bank.EntryNotice(2,17,info.array);}CHECK(!bank.Narrators.size());bank.Close();bank.Close();CHECK(!bank.Entries.size() && !bank.TextEntryBuffer.size() && !bank.GroupEntryBuffer.size());++entryCases;
  }
  CHECK(chars==g_CCharStringInstanceCount_013BD800 && wides==g_CWideStringInstanceCount_013BCA20);
 }
 printf("NATIVE_TEXT_METADATA_BOUNDARY_PASS size=512 entry=512 readonly_noop=512 version=106\n");return 0;
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
