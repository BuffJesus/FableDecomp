#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
static unsigned long PointerBytes(const CCountedPointer<CBankFileAsyncData>& pointer){if(pointer.Data)return sizeof(pointer)+pointer.Data->Sizeof();return sizeof(pointer);}
inline unsigned long NGameText::CGroupEntry::Sizeof() const
{
    unsigned long size=sizeof(*this)+Entries.capacity()*sizeof(long);
    size+=PointerBytes(PFileData)-sizeof(PFileData);
    return size;
}
void NGameText::CGroupEntry::ReadFromDataStream(CDataInputStream& stream)
{
    long count=stream.ReadValue<long>();
    Entries.resize(count,0L);
    for(long i=0;i<count;++i)Entries[i]=stream.ReadValue<long>();
    SetResourceSize(Sizeof());
}
