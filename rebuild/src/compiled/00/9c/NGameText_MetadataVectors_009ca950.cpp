// Typed text-bank metadata recovery; original callbacks and guarded vector models verified.
#include <new>
#define FABLE_STLP_FILL_CALL __fastcall
#include "fable_game_text_bank.h"
#include "fable_stlport_algobase.h"
namespace _STL {
// Original 009C9C60 remains a separate fastcall helper in this TU.
template<> __declspec(noinline) void __fastcall fill<NGameText::CDataBankEntry**,NGameText::CDataBankEntry*>(NGameText::CDataBankEntry** first,NGameText::CDataBankEntry** last,NGameText::CDataBankEntry* const& value)
{
    for(;first!=last;++first)*first=value;
}
}
#include "fable_stlport_vector_impl.h"
namespace _STL {
template void vector<NGameText::CDataBankEntry*>::resize(size_t,NGameText::CDataBankEntry* const&);
template void vector<NGameText::CDataBankEntry*>::_M_insert_overflow(NGameText::CDataBankEntry**,NGameText::CDataBankEntry* const&,const __true_type&,size_t,bool);
template void vector<NGameText::CDataBankTextEntry>::reserve(size_t);
template void vector<NGameText::CDataBankGroupEntry>::reserve(size_t);
template void vector<NGameText::CDataBankTextEntry>::push_back(const NGameText::CDataBankTextEntry&);
template void vector<NGameText::CDataBankGroupEntry>::push_back(const NGameText::CDataBankGroupEntry&);
}
