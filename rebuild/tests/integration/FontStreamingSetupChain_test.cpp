#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_bank_async_entry.h"
#include "fable_font_data_banks.h"
#include "test_support/base_class_stubs.h"
typedef CCountedPointer<CBankFileAsyncEntry> Ptr;
typedef _STL::map<CCharString,unsigned long>::value_type Value;typedef _STL::_Rb_tree_node<Value> Node;
static union{double align;char data[0x190];} bankStore;
static union{double align;char data[24];} slots[4];
static bool freed[4];static unsigned sizes[4];static int total,attempt,fail,ctors,dtors,phase;
static CStreamingFontDataBank* owner;static CBankFileAsyncEntry* newData;static CCPPointerInfo* newInfo;
struct Ord{const char* text;};static Ord glyph={"GlyphData"};static Node header,node;static char symbols[64];
static int slot(void* p){for(int i=0;i<total;++i)if(p==slots[i].data)return i;abort();return 0;}
void* __cdecl operator new(size_t size){if(!owner)return malloc(size);++attempt;if(attempt==fail)return 0;if(total>=4)abort();void* p=slots[total].data;sizes[total]=size;freed[total++]=false;if(size==16)newData=(CBankFileAsyncEntry*)p;else if(size==12)newInfo=(CCPPointerInfo*)p;else abort();return p;}
void __cdecl operator delete(void* p){if(!owner){free(p);return;}int i=slot(p);if(freed[i])abort();freed[i]=true;}
CBaseClassNonCopyable::CBaseClassNonCopyable(){if(owner)++ctors;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){if(owner)++dtors;}
CCharString::CCharString(){storage_=0;}
CCharString::CCharString(const char* p,long length){if(!owner||phase++!=0||strcmp(p,"GlyphData")||length!=-1)abort();storage_=(CCharStringData*)&glyph;}
CCharString::~CCharString(){if(owner&&phase++!=1)abort();}
int main(){unsigned long alignments[]={1,3,16,4096};int cases=0;CBankFile::CRuntimeData runtime[4];unsigned long packed=0x00813f11;
for(int mode=0;mode<3;++mode)for(int ix=0;ix<4;++ix)for(int a=0;a<4;++a)for(int f1=0;f1<3;++f1)for(int f2=0;f2<3;++f2){if(mode&&ix==0)continue;
 memset(&bankStore,0,sizeof(bankStore));owner=(CStreamingFontDataBank*)&bankStore;owner->Alignment=alignments[a];owner->SymbolIndices._M_t._M_header._M_data=&header;header._M_parent=ix?&node:0;node._M_parent=&header;node._M_left=node._M_right=0;*(void**)&node._M_value_field.first=&glyph;node._M_value_field.second=ix;
 for(int k=0;k<4;++k)runtime[k].DataSize=k*33+17;owner->RuntimeData._M_start=runtime;
 owner->PackedDataSize.Size=mode?3:0;owner->PackedDataSize.Bits=mode==1?8:0;owner->PackedDataSize.Bias=mode==2?77:0;owner->PackedDataSize.PackedInts=&packed;
 total=ctors=dtors=0;CBankFileAsyncEntry* expectedData=0;CCPPointerInfo* expectedInfo=0;
 for(int pass=0;pass<2;++pass){attempt=phase=0;fail=pass?f2:f1;newData=0;newInfo=0;owner->InitStreamingEntry(*(CSymbolMap*)symbols);
  if(expectedInfo!=newInfo){expectedData=newData;expectedInfo=newInfo;}
  if(phase!=2||owner->streamingEntry.Data!=expectedData||owner->streamingEntry.Info!=expectedInfo)return 1;
  if(expectedInfo){if(expectedInfo->RefCount!=1||expectedInfo->Data!=expectedData||freed[slot(expectedInfo)]||freed[slot(expectedData)])return 2;
   unsigned long size=mode==0?runtime[ix].DataSize:mode==1?(packed>>((ix-1)*8))&255:77;
   unsigned long expected=((size+alignments[a]-1)/alignments[a])*alignments[a];
   if(expectedData->Index!=(unsigned long)ix||&expectedData->BankFile!=owner||expectedData->PaddedSize!=expected)return 3;
  }
 }
 owner->streamingEntry.~Ptr();
 for(int i=0;i<total;++i)if(!freed[i]){if(sizes[i]!=16)return 4;delete (CBankFileAsyncEntry*)slots[i].data;}
 if(dtors!=ctors)return 5;
 owner=0;++cases;
}
printf("FONT_STREAMING_SETUP_CHAIN_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
