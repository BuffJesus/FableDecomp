#define FABLE_STLP_FILL_CALL __fastcall
#include "fable_game_text_bank.h"
#include "fable_stlport_vector_impl.h"
namespace _STL {
template<> __forceinline CBankFileEntryUpdateData** __copy_backward_ptrs(CBankFileEntryUpdateData** first,CBankFileEntryUpdateData** last,CBankFileEntryUpdateData** result,const __true_type&) {
ptrdiff_t bytes=reinterpret_cast<char*>(last)-reinterpret_cast<char*>(first);
if(bytes>0)memmove(reinterpret_cast<char*>(result)-bytes,first,bytes);
return reinterpret_cast<CBankFileEntryUpdateData**>(reinterpret_cast<char*>(result)-bytes);
}
template void vector<CBankFileEntryUpdateData*>::_M_insert_overflow(CBankFileEntryUpdateData**,CBankFileEntryUpdateData* const&,const __true_type&,size_t,bool);
template void vector<CBankFileEntryUpdateData*>::_M_fill_insert(CBankFileEntryUpdateData**,size_t,CBankFileEntryUpdateData* const&);
}
