#include "fable_game_text_bank.h"
#include "fable_stlport_algobase.h"

namespace _STL
{
    template<> __forceinline NGameText::CDataBankTextEntry*
    vector<NGameText::CDataBankTextEntry>::erase(
        NGameText::CDataBankTextEntry* first, NGameText::CDataBankTextEntry* last)
    {
        NGameText::CDataBankTextEntry* finish = copy(last, _M_finish, first);
        _Destroy(finish, _M_finish);
        _M_finish = finish;
        return first;
    }

    template<> __forceinline NGameText::CDataBankGroupEntry*
    vector<NGameText::CDataBankGroupEntry>::erase(
        NGameText::CDataBankGroupEntry* first, NGameText::CDataBankGroupEntry* last)
    {
        NGameText::CDataBankGroupEntry* finish = copy(last, _M_finish, first);
        _Destroy(finish, _M_finish);
        _M_finish = finish;
        return first;
    }
}

void NGameText::CDataBank::CloseTextAndGroups()
{
    Entries.clear();
    TextEntryBuffer.clear();
    GroupEntryBuffer.clear();
}

