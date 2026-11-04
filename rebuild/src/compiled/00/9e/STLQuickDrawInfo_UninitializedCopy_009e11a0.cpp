#include <new>
#include "fable_quick_draw_info.h"
#include "fable_stlport_uninitialized.h"

// Retail 009E11A0: out-of-line fastcall copy-construct range for CQuickDrawTriInfo,
// called by vector<CQuickDrawTriInfo>::_M_insert_overflow (009E1750).
namespace _STL {
template CQuickDrawTriInfo* FABLE_FASTCALL __uninitialized_copy(
    CQuickDrawTriInfo*, CQuickDrawTriInfo*, CQuickDrawTriInfo*, const __false_type&);
}
