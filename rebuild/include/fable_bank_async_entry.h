#pragma once
#include "fable_game_text_bank.h"
// Ego_r.pdb: 16-byte CBaseClassNonCopyable descendant; retail vtable 0129B880.
class CBankFileAsyncEntry : public CBaseClassNonCopyable {
public:
 virtual ~CBankFileAsyncEntry() {}
 void ReadDataNonAsync(unsigned long offset,void* data,unsigned long size);
 // ?BeginReadData@...@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KPAXKKW4EThreadedFilePriority@@@Z,
 // retail 009D6060: read into a caller-supplied buffer.
 CCountedPointer<CBankFileAsyncData> BeginReadData(
     unsigned long offset,void* buffer,unsigned long size,unsigned long bufferSize,EThreadedFilePriority priority);
 // Ego_r72A640; retail9D6100: hidden counted-pointer return plus three arguments.
 CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(
     unsigned long offset,unsigned long size,EThreadedFilePriority priority);
 CBankFileAsync& BankFile;
 unsigned long Index;
 unsigned long PaddedSize;
private:
 void SetupRead(unsigned long, CBankFileAsyncData&, unsigned long, unsigned long, unsigned long, EThreadedFilePriority);
 friend class CBankFileAsync;
 // ??0CBankFileAsyncEntry@@AAE@AAVCBankFileAsync@@K@Z, retail 009D5260 (out of line;
 // auto-inlined into GetAsyncEntry 009D58D0 in the same TU).
 CBankFileAsyncEntry(CBankFileAsync& bank,unsigned long index);
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsyncEntry)==16);
