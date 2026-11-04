#include "fable_game_text_bank.h"
void NGameText::CDataBank::BeginLoadingText(unsigned long index) const
{
    const CDataBankTextEntry* entry = GetTextInfo(index);
    if (entry) entry->BeginLoading(index, *const_cast<CDataBank*>(this));
}
void NGameText::CDataBank::BeginLoadingGroup(unsigned long index) const
{
    const CDataBankGroupEntry* entry = GetGroupInfo(index);
    if (entry) entry->BeginLoading(index, *const_cast<CDataBank*>(this));
}
