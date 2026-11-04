#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
static unsigned long PointerBytes(const CCountedPointer<CBankFileAsyncData>& pointer){if(pointer.Data)return sizeof(pointer)+pointer.Data->Sizeof();return sizeof(pointer);}
inline unsigned long NGameText::CGroupEntry::Sizeof() const
{
    unsigned long size=sizeof(*this)+Entries.capacity()*sizeof(long);
    size+=PointerBytes(PFileData)-sizeof(PFileData);
    return size;
}
#include "fable_counted_pointer_inline.h"
NGameText::CGroupEntry::CGroupEntry(const CCountedPointer<CBankFileAsyncData>& data,CDataBankGroupEntry* owner):PFileData(data),Owner(owner){SetResourceSize(Sizeof());}
NGameText::CTextEntry::CTextEntry(const CCountedPointer<CBankFileAsyncData>& data,CDataBankTextEntry* owner):PFileData(data),Owner(owner){SetResourceSize(Sizeof());}
