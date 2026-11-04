#include "fable_font_data_banks.h"
#include "fable_bank_entry_update.h"
#include <stdio.h>
#include <string.h>
struct BaseAccess:CBankFile {
 static unsigned long Adjust(CBankFile* p){return ((BaseAccess*)p)->CBankFile::AdjustUpdateEntries();}
 static bool Disk(CBankFile* p,unsigned long a,unsigned long b){return ((BaseAccess*)p)->CBankFile::IsDiskFile(a,b);}
};
struct FontAccess:CFontDataBank {
 static unsigned long Version(CFontDataBank* p){return ((FontAccess*)p)->CFontDataBank::GetDataFormatVersion();}
 static bool Disk(CFontDataBank* p,unsigned long a,unsigned long b){return ((FontAccess*)p)->CFontDataBank::IsDiskFile(a,b);}
};
struct StreamAccess:CStreamingFontDataBank {
 static unsigned long Version(CStreamingFontDataBank* p){return ((StreamAccess*)p)->CStreamingFontDataBank::GetDataFormatVersion();}
 static bool Disk(CStreamingFontDataBank* p,unsigned long a,unsigned long b){return ((StreamAccess*)p)->CStreamingFontDataBank::IsDiskFile(a,b);}
 static unsigned long Adjust(CStreamingFontDataBank* p){return ((StreamAccess*)p)->CStreamingFontDataBank::AdjustUpdateEntries();}
};
static bool valid(unsigned i,int pattern){return pattern==0||pattern==1&&i%2!=0||pattern==2&&i%3==0;}
int main(){int cases=0;
 for(unsigned n=0;n<=12;++n)for(int shortRuntime=0;shortRuntime<2;++shortRuntime)for(int packed=0;packed<3;++packed)for(int pattern=0;pattern<3;++pattern)for(int prior=0;prior<2;++prior){
  union{double align;char data[0x190];} mem;memset(&mem,0xa5,sizeof(mem));CBankFile* bank=(CBankFile*)&mem;CBankFile::CRuntimeData runtime[16];unsigned long words[16]={0};
  union{double align;char bytes[16*0x20];} updateMemory;memset(&updateMemory,0x6b,sizeof(updateMemory));CBankFileEntryUpdateData* updates[16];
  unsigned count=shortRuntime?n/2:n;for(unsigned i=0;i<16;++i){runtime[i].Valid=valid(i,pattern);updates[i]=(CBankFileEntryUpdateData*)(updateMemory.bytes+i*0x20);updates[i]->RequiresUpdate=prior!=0;if(i){unsigned value=valid(i,2-pattern);if(packed==1)words[(i-1)/32]|=value<<((i-1)%32);else words[i-1]=value?0x80000000:0;}}
  bank->Size=n;bank->RuntimeData._M_start=runtime;bank->RuntimeData._M_finish=runtime+count;bank->UpdateData._M_start=updates;bank->PackedValid.Size=packed!=0;bank->PackedValid.Bits=packed==1?1:32;bank->PackedValid.Bias=0;bank->PackedValid.PackedInts=words;
  char bankBefore[0x190];memcpy(bankBefore,&mem,sizeof(mem));unsigned indices[]={0,1,n,n+1,0xffffffff};
  for(int q=0;q<5;++q)for(int requested=0;requested<2;++requested){unsigned index=indices[q];bool exists=index&&index<n&&valid(index,packed?2-pattern:pattern);if(bank->EntryExists(index)!=exists)return 1;
   bool allowed=index<count&&valid(index,pattern);bool old=allowed?updates[index]->RequiresUpdate:false;if(bank->EntrySetUpdate(index,requested!=0)!=old)return 2;if(allowed&&updates[index]->RequiresUpdate!=(requested!=0))return 3;
   if(BaseAccess::Adjust(bank)!=0||!BaseAccess::Disk(bank,index,n)||FontAccess::Version((CFontDataBank*)bank)!=105||FontAccess::Disk((CFontDataBank*)bank,index,n)||StreamAccess::Version((CStreamingFontDataBank*)bank)!=109||StreamAccess::Disk((CStreamingFontDataBank*)bank,index,n))return 4;
  }
  for(unsigned i=0;i<16;++i)updates[i]->RequiresUpdate=prior!=0;
  for(int pass=0;pass<2;++pass){unsigned expected=0;bool wanted[16];for(unsigned i=0;i<16;++i)wanted[i]=updates[i]->RequiresUpdate;
   for(unsigned i=1;i<n;++i)if(valid(i,packed?2-pattern:pattern)){bool allowed=i<count&&valid(i,pattern);if(!allowed||!wanted[i])++expected;if(allowed)wanted[i]=true;}
   if(StreamAccess::Adjust((CStreamingFontDataBank*)bank)!=expected)return 5;
   for(unsigned i=0;i<16;++i)if(updates[i]->RequiresUpdate!=wanted[i])return 6;
  }
  if(memcmp(bankBefore,&mem,sizeof(mem)))return 7;
  for(unsigned i=0;i<sizeof(updateMemory);++i)if(i%0x20!=0x14&&(unsigned char)updateMemory.bytes[i]!=0x6b)return 8;
  ++cases;
 }
 printf("BANK_UPDATE_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
