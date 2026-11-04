// STLport __introsort_loop<CWideString*, CWideString, int,
// less<CWideString>> @ 0x009B7CF0.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_introsort.h"

template void FABLE_FASTCALL _STL::__introsort_loop(
    CWideString* first, CWideString* last, CWideString* tag,
    int depthLimit, _STL::less<CWideString> comparator);
