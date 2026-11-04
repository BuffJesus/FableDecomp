// Retail 009D0880: header validation, rebuild/recovery routing and entry parsing.
// The exception constructor must be visible in this translation unit for the
// retail inlining context. This provider also emits its existing exact 009D1E70
// body; select one constructor provider when linking a composition.
// Opcode grade requires independently verified literal operands (retail has no
// PE relocation entries for this span); the gate retains the conservative score.
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "fable_disk_file.h"
#include "fable_exception.h"
#include "fable_bank_file_exception.h"



CBankFileException::CBankFileException(CCharString errortext,
                                      EBankFileExceptionType type)
{
    static_cast<const char*>(errortext);
    Type = type;
}

static __forceinline int CompareMagic(CDataInputStream& stream) {char signature[]="BBBB";char header[4];stream.Read(header,4);return memcmp(header,signature,4);}
bool CBankFile::ReadFileInformation(bool enable_exceptions)
{
 CCharString filename=Filename.ToCharString();
 CFileDataInputStream stream(BankFile.Data,0x4000);
 stream.SetPosition(0);
 if(stream.IsEOF() || CompareMagic(stream)!=0) {
  if(enable_exceptions)throw CBankFileException(filename+" is not a valid resource file!",CBankFileException::BANK_IS_CORRUPT);
  return false;
 }
 unsigned long version=stream.ReadValue<unsigned long>();
 if(version!=BankFormatVersion) {
  if(ReadOnly) {
   if(enable_exceptions)throw CBankFileException(filename+" has the wrong bank format! (should be "+GFIntToCharString(BankFormatVersion)+" not "+GFIntToCharString(version)+")",CBankFileException::BANK_IS_INCORRECT_VERSION);
   return false;
  }
  CCharString message("Bank file version has changed, requiring rebuild");
  if(ProgressCallback)static_cast<CProgressCallback*>(ProgressCallback)->NotifyBankLogMessage(message);
  return true;
 }
 unsigned long format=stream.ReadValue<unsigned long>();
 if(format!=GetDataFormatVersion()) {
  if(ReadOnly) {
   if(enable_exceptions)throw CBankFileException(filename+" has the wrong version number! (should be "+GFIntToCharString(GetDataFormatVersion())+" not "+GFIntToCharString(format)+")",CBankFileException::BANK_IS_INCORRECT_VERSION);
   return false;
  }
  CCharString message("Bank data version has changed, requiring rebuild");
  if(ProgressCallback)static_cast<CProgressCallback*>(ProgressCallback)->NotifyBankLogMessage(message);
  return true;
 }
 unsigned long state=stream.ReadValue<unsigned long>();
 unsigned long alignment=stream.ReadValue<unsigned long>();
 if(alignment!=Alignment) {
  if(ReadOnly)Alignment=alignment;
  else {
   CCharString message("Data alignment has changed, requiring rebuild");
   if(ProgressCallback)static_cast<CProgressCallback*>(ProgressCallback)->NotifyBankLogMessage(message);
   return true;
  }
 }
 unsigned long max_index=stream.ReadValue<unsigned long>();
 unsigned long entry_count=stream.ReadValue<unsigned long>();
 unsigned long position=stream.ReadValue<unsigned long>();
 HeaderSize=stream.GetPosition();
 FileSize=BankFile->GetLength();
 if(state!=1) {
  if(ReadOnly) {
   if(enable_exceptions)throw CBankFileException(CBankFileException::BANK_IS_CORRUPT);
   return false;
  }
  if(!ReadRecoveryFile(state)) {
   CCharString message("Previous bank update was interrupted, requiring rebuild");
   if(ProgressCallback)static_cast<CProgressCallback*>(ProgressCallback)->NotifyBankLogMessage(message);
  }
  return true;
 }
 stream.SetPosition(position);
 return ReadEntryList(stream,max_index,entry_count);
}
