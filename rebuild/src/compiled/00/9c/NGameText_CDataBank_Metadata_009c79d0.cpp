// Typed text-bank metadata recovery; original callbacks and guarded vector models verified.
#include "fable_game_text_bank.h"
#include "fable_bank_file_data_type_count.h"
unsigned long NGameText::CDataBank::GetDataFormatVersion(){return 106;}
void NGameText::CDataBank::NotifyBankSize(unsigned long size,const CArray<CBankFileDataTypeCount>& counts)
{
    if(ReadOnly){
        unsigned long groups=0;
        CDataBankEntry* empty=0;
        Entries.resize(size,empty);
        unsigned long texts=0;
        unsigned long count=counts.size();
        const CBankFileDataTypeCount* p=counts.begin();
        while(count){
            unsigned long type=p->DataType,n=p->Count;
            if(type==0)texts+=n;
            else if(type==1)groups+=n;
            ++p;--count;
        }
        TextEntryBuffer.reserve(texts);
        GroupEntryBuffer.reserve(groups);
    }
}
