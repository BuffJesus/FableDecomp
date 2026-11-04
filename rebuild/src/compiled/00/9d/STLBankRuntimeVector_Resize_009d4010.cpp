// Verified original resize root. Fill insertion remains a separate native dependency.
#include "fable_game_text_bank.h"
#include "fable_stlport_char_string_vector.h"
#include "fable_stlport_vector_impl.h"
template<> inline CBankFile::CRuntimeData* _STL::vector<CBankFile::CRuntimeData>::erase(CBankFile::CRuntimeData* first, CBankFile::CRuntimeData* last) {CBankFile::CRuntimeData* out=_STL::copy(last,this->_M_finish,first);this->_M_finish=out;return first;}
template<> void _STL::vector<CBankFile::CRuntimeData>::_M_fill_insert(CBankFile::CRuntimeData*,size_t,const CBankFile::CRuntimeData&);
template void _STL::vector<CBankFile::CRuntimeData>::resize(size_t,const CBankFile::CRuntimeData&);
