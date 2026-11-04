// Native bank symbol-index map; recovered STLport unsigned-long instantiation.
#pragma optimize("s",on)

#include "fable_stlport_tree.h"
#include "fable_string.h"
#include "fable_stlport_tree_bounds.h"

namespace _STL {
template _Rb_tree<CCharString, pair<const CCharString, unsigned long>, _Select1st<pair<const CCharString, unsigned long> >,
                  less<CCharString>, allocator<pair<const CCharString, unsigned long> > >::_Link_type
_Rb_tree<CCharString, pair<const CCharString, unsigned long>, _Select1st<pair<const CCharString, unsigned long> >,
         less<CCharString>, allocator<pair<const CCharString, unsigned long> > >::_M_lower_bound(const CCharString& __k) const;
}
