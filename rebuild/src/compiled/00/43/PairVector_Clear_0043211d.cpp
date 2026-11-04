#define FABLE_UNIT_INLINE
#include "fable_def_class_base.h"

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> Pair;

namespace _STL {
template <> struct __type_traits<Pair> {
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};
}

template void _STL::vector<Pair>::_M_clear();
