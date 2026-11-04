// STLport sort<CWideString*, less<CWideString> > @ 0x009B8440.
// The definition path scans sort each pattern's lower-case filenames here.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_algo_impl.h"

template void FABLE_FASTCALL _STL::sort(
    CWideString* first, CWideString* last, _STL::less<CWideString> comparator);
