#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_bank_async_entry.h"
#include "test_support/base_class_stubs.h"
static union{double align;char bytes[16];} entryStorage;
static union{double align;char bytes[12];} infoStorage;
static union{double align;char bytes[0x164];} banks[2];
static CBankFileAsync* owner;static unsigned long dataSize,indexValue;static int fail,mutation,allocations,baseCalls,destroyed,freed;
static void* deleted[2];
void* __cdecl operator new(size_t size){if(!owner)return malloc(size);++allocations;if((allocations==1&&size!=16)||(allocations==2&&size!=12)||allocations>2)abort();if(allocations==fail)return 0;return allocations==1?(void*)&entryStorage:(void*)&infoStorage;}
void __cdecl operator delete(void* p){if(!owner){free(p);return;}if(freed>=2)abort();deleted[freed++]=p;}
CBaseClassNonCopyable::CBaseClassNonCopyable(){if(this!=(void*)&entryStorage)abort();++baseCalls;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){if(this!=(void*)&entryStorage)abort();++destroyed;}
unsigned long CBankFile::GetEntryDataSize(unsigned long index) const {CBankFileAsyncEntry* entry=(CBankFileAsyncEntry*)&entryStorage;if(this!=owner||index!=indexValue||entry->Index!=index||&entry->BankFile!=owner)abort();if(mutation)*(CBankFileAsync**)(entryStorage.bytes+4)=(CBankFileAsync*)&banks[1];return dataSize;}
int main(){unsigned long aligns[]={1,2,3,4,16,4096,0xffffffff},sizes[]={0,1,2,3,15,16,17,0xffffffff,0xfffffffe};int cases=0;
for(int a=0;a<7;++a)for(int s=0;s<9;++s)for(int ix=0;ix<3;++ix)for(int f=0;f<3;++f)for(int m=0;m<2;++m){
 memset(&entryStorage,0xcc,sizeof(entryStorage));memset(&infoStorage,0xcc,sizeof(infoStorage));owner=(CBankFileAsync*)&banks[0];owner->Alignment=aligns[a];((CBankFileAsync*)&banks[1])->Alignment=5;
 indexValue=ix==0?0:ix==1?7:0xffffffff;dataSize=sizes[s];mutation=m;fail=f;allocations=baseCalls=destroyed=freed=0;
 {CCountedPointer<CBankFileAsyncEntry> p=owner->GetAsyncEntry(indexValue);
  if(f==1){if(p.Data||p.Info||allocations!=1||baseCalls)return 1;}
  else {if(p.Data!=(void*)&entryStorage||allocations!=2||baseCalls!=1)return 2;
   unsigned long align=m?5:aligns[a],expected=((dataSize+align-1)/align)*align;
   if(p.Data->Index!=indexValue||p.Data->PaddedSize!=expected||&p.Data->BankFile!=(CBankFileAsync*)&banks[m])return 3;
   if(f==2){if(p.Info)return 4;delete p.Data;p.Data=0;}
   else if(p.Info!=(void*)&infoStorage||p.Info->RefCount!=1||p.Info->Data!=p.Data||!p.Info->DeleteFunc)return 5;
  }
 }
 if(destroyed!=(f!=1)||freed!=(f==0?2:f==2?1:0))return 6;
 if(f!=1&&deleted[0]!=&entryStorage)return 7;
 if(f==0&&deleted[1]!=&infoStorage)return 8;
 owner=0;++cases;
}
printf("BANK_ASYNC_ENTRY_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
