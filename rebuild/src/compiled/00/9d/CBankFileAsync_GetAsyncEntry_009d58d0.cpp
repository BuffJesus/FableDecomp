#include "fable_bank_async_entry.h"
template<class T> CCountedPointer<T>::CCountedPointer(T* data):Data(data),Info(data?new CCPPointerInfo(DeleteData,data):0) {}
template<class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer<T>& other):Data(other.Data),Info(other.Info) {if(Info)++Info->RefCount;}
CBankFileAsyncEntry::CBankFileAsyncEntry(CBankFileAsync& bank,unsigned long index):BankFile(bank),Index(index) {
 PaddedSize=BankFile.Align(BankFile.GetEntryDataSize(index));
}
CCountedPointer<CBankFileAsyncEntry> CBankFileAsync::GetAsyncEntry(unsigned long index) {
 CCountedPointer<CBankFileAsyncEntry> entry(new CBankFileAsyncEntry(*this,index));
 return entry;
}

// Keep the retail call boundary while sharing this provider.
__declspec(noinline) unsigned long CBankFile::Align(unsigned long size) const {return ((size+Alignment-1)/Alignment)*Alignment;}
