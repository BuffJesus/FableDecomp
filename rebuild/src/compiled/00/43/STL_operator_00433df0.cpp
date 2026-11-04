// _STL::vector<CWideString>::operator=(const vector&) @ 0x00433DF0 (STLport;
// CDefinitionManager::SetSymbolPathList / SetCompilePathList tail-call it with
// the member vector in ecx). 369 B: the oracle row that ran on to 413 B had
// swallowed the padding and the two SetXxxPathList thunks after the ret.
//
// Retail's /O2 unit expands STLport's range wrappers on every branch:
// _M_allocate_and_copy (malloc plus the copy-construct loop with the
// placement-new null test), _M_clear (the destroy loop and free), _Destroy
// over the surplus and __copy_ptrs' counted assignment loop. Only the last
// __uninitialized_copy stays out of line (fastcall, tag in the dead argument
// slot). VC7.1 keeps a wrapper that is called on a conditional path out of
// line unless it is forced, so this unit sets FABLE_UNIT_INLINE (rebuild_abi.h)
// to __forceinline. The helper bodies come from fable_stlport_uninitialized.h.
#define FABLE_UNIT_INLINE __forceinline
#include "fable_string.h"
#include "fable_stlport_vector_assign.h"
#include "fable_stlport_uninitialized.h"

#include <new>

template _STL::vector<CWideString>& _STL::vector<CWideString>::operator=(const _STL::vector<CWideString>& __x);