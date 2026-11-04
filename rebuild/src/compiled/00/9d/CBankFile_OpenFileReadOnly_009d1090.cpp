#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
inline CAFile::CAFile() {}
extern void FABLE_FASTCALL CloseBankFileImpl(CBankFile*);
// Retail009D1090: initialize flags, open the disk file, parse metadata, notify.
bool CBankFile::OpenFileReadOnly(const CWideString& filename,unsigned long flags)
{
 ReadOnly=true;
 OpenFlags=flags;
 ChangeCount=0;
 FileValid=false;
 WrittenToFile=false;
 Solid=false;
 UpdatingFlag=false;
 RetailMode=false;
 if(!CAFile::PathExists(filename))return false;
 BankFile.Reset(new CDiskFileWin32);
 BankFile->Open(filename,FILE_ACCESS_READ,4);
 Filename=filename;
 if(!ReadFileInformation(!(flags&2))) {
  CloseBankFileImpl(this);
  return false;
 }
 NotifyBankOpened();
 return true;
}
