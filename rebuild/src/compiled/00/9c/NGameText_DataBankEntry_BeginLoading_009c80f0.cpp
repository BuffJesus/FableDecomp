#include "fable_game_text_bank.h"
#include "fable_bank_async_entry.h"
#include "fable_counted_pointer_inline.h"
extern EThreadedFilePriority g_NativeTextLoadingPriority_01396314;
void NGameText::CDataBankGroupEntry::BeginLoading(unsigned long index,CDataBank& bank) const
{
 if(PGroup.IsNull()){
  CCountedPointer<CBankFileAsyncEntry> file=bank.GetAsyncEntry(index);
  CCountedPointer<CBankFileAsyncData> data=file->BeginReadDataAutoAlloc(0,bank.GetEntryDataSize(index),g_NativeTextLoadingPriority_01396314);
  CGroupEntry* entry=new CGroupEntry(data,const_cast<CDataBankGroupEntry*>(this));
  CIVCountedPointer<CGroupEntry>& pointer=const_cast<CIVCountedPointer<CGroupEntry>&>(PGroup);
  pointer.Release();pointer.data_=entry;
  bank.AddResource(*entry);bank.ResourceList.MakeFreeSpace(0);
 }
 PGroup->TouchResource();
}
void NGameText::CDataBankTextEntry::BeginLoading(unsigned long index,CDataBank& bank) const
{
 if(PText.IsNull()){
  CCountedPointer<CBankFileAsyncEntry> file=bank.GetAsyncEntry(index);
  CCountedPointer<CBankFileAsyncData> data=file->BeginReadDataAutoAlloc(0,bank.GetEntryDataSize(index),g_NativeTextLoadingPriority_01396314);
  CTextEntry* entry=new CTextEntry(data,const_cast<CDataBankTextEntry*>(this));
  CIVCountedPointer<CTextEntry>& pointer=const_cast<CIVCountedPointer<CTextEntry>&>(PText);
  pointer.Release();pointer.data_=entry;
  bank.AddResource(*entry);bank.ResourceList.MakeFreeSpace(0);
 }
 PText->TouchResource();
}
