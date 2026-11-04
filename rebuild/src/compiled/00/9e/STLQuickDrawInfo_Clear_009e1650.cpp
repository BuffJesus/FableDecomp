#include <new>
#include "fable_quick_draw_info.h"
#include "fable_stlport_uninitialized.h"

// Retail 009E1650: vector<CQuickDrawTriInfo>::_M_clear, destroying each element
// (texture Uninitialise, wide-string release) before freeing storage.
namespace _STL {
template void vector<CQuickDrawTriInfo>::_M_clear();
}
