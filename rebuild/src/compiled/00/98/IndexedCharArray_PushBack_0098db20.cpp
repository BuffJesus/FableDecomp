#include <new>
#include "fable_indexed_array.h"
namespace _STL {
template<> void vector<CCharString>::_M_insert_overflow(CCharString*,const CCharString&,const __false_type&,size_t,bool);
}
#include "fable_stlport_vector_impl.h"
template<> void CIndexedArray<CCharString>::push_back(const CCharString& value) {
    IndexMap.insert(_STL::map<CCharString,long>::value_type(value,Array.size()));
    Array.push_back(value);
}
