// Verified original resize root. Fill insertion remains a separate native dependency.
#include "fable_game_text_bank.h"
#include "fable_stlport_char_string_vector.h"
#include "fable_stlport_vector_impl.h"
template<> void _STL::vector<CBankFileEntryUpdateData*>::_M_fill_insert(CBankFileEntryUpdateData**,size_t,CBankFileEntryUpdateData* const&);
template void _STL::vector<CBankFileEntryUpdateData*>::resize(size_t,CBankFileEntryUpdateData* const&);
