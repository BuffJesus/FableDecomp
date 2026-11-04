#include <new>
#include "fable_4d_vector.h"
#include "fable_stlport_algobase.h"
namespace _STL {
template<> C4DVector* vector<C4DVector>::erase(C4DVector* first,C4DVector* last)
{
 C4DVector* finish=__copy(last,this->_M_finish,first,random_access_iterator_tag(),(ptrdiff_t*)0);
 _Destroy(finish,this->_M_finish);
 this->_M_finish=finish;
 return first;
}
}
