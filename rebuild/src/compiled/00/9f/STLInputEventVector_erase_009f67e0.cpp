#include "fable_input_manager.h"
#include "fable_stlport_algobase.h"
// STLport vector<CInputEvent>::erase(first, last): copy the tail down with the
// counted random-access __copy, then _Destroy what is left past the new end.
template <> CInputEvent* _STL::vector<CInputEvent>::erase(CInputEvent* first, CInputEvent* last)
{
    CInputEvent* end = _STL::__copy(last, this->_M_finish, first, _STL::random_access_iterator_tag(), (ptrdiff_t*)0);
    _STL::_Destroy(end, this->_M_finish);
    this->_M_finish = end;
    return first;
}
