#define FABLE_BASIC_STRING_COMPARE_NOINLINE
#include "fable_stlport_tree.h"
#include "fable_string.h"
#include "fable_stlport_tree_bounds.h"

namespace _STL {
template _Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
                  less<CCharString>, allocator<pair<const CCharString, long> > >::_Link_type
_Rb_tree<CCharString, pair<const CCharString, long>, _Select1st<pair<const CCharString, long> >,
         less<CCharString>, allocator<pair<const CCharString, long> > >::_M_lower_bound(const CCharString& __k) const;
}
