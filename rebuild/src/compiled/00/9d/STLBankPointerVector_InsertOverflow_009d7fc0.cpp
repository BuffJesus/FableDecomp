#include <new>
#include "fable_carray.h"
#include "fable_stlport_vector_impl.h"
class CBankFileAsync;
// Retail 009D7FC0: POD growth path behind CLoadingMemoryFailureHandler::RegisterBank (009D6420).
template void _STL::vector<CBankFileAsync*>::_M_insert_overflow(CBankFileAsync**,CBankFileAsync* const&,const _STL::__true_type&,size_t,bool);
