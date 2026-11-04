// Typed text-bank metadata recovery; original callbacks and guarded vector models verified.
#include <new>
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "fable_stlport_char_string_vector.h"
#include "fable_stlport_vector_impl.h"
void NGameText::CDataBank::NotifyEntryAdded(unsigned long type,unsigned long index,const CArray<char>& info)
{
    if(ReadOnly){
        CDataBankEntry* empty=0;
        while(index>=Entries.size())Entries.push_back(empty);
        if(type==0){
            CDataBankTextEntry entry;
            entry.NarratorIndex=*reinterpret_cast<const long*>(info.GetAsCArray());
            TextEntryBuffer.push_back(entry);
            Entries[index]=&TextEntryBuffer.back();
        }else if(type==1){
            CDataBankGroupEntry entry;
            GroupEntryBuffer.push_back(entry);
            Entries[index]=&GroupEntryBuffer.back();
        }else if(type==2){
            CMemoryDataInputStream stream(info.GetAsCArray(),info.size());
            unsigned long count=stream.ReadULONG();
            Narrators.resize(count);
            for(unsigned long i=0;i<count;++i)Narrators[i]=stream.ReadNullTerminatedString();
        }
        CBankFile::NotifyEntryAdded(type,index,info);
    }
}
