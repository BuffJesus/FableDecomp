#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "fable_counted_pointer_inline.h"
void NGameText::CDataBankGroupEntry::BuildGroup() const
{
 PGroup->TouchResource();
 if(PGroup->PFileData.Data){
  if(!PGroup->PFileData->IsFinished()) PGroup->PFileData->WaitUntilFinished();
  CMemoryDataInputStream stream(PGroup->PFileData->GetData(),PGroup->PFileData->Length);
  PGroup->ReadFromDataStream(stream);
  PGroup->PFileData.ReleaseData();
 }
}
void NGameText::CDataBankTextEntry::BuildText() const
{
 PText->TouchResource();
 if(PText->PFileData.Data){
  if(!PText->PFileData->IsFinished()) PText->PFileData->WaitUntilFinished();
  CMemoryDataInputStream stream(PText->PFileData->GetData(),PText->PFileData->Length);
  PText->ReadFromDataStream(stream);
  PText->PFileData.ReleaseData();
 }
}
