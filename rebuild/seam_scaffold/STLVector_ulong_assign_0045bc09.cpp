// SEAM SCAFFOLD, not a landing: _STL::vector<unsigned long>::operator=
// (const vector&) @ 0x0045BC09. Behaviourally faithful STLport 4.6
// assignment (fable_stlport_vector_assign.h); 163 B against retail's 184.
//
// Why not landed: retail's size-optimised unit keeps _M_clear (0x0045BCE4),
// _M_allocate_and_copy (0x0045BCF2) and the POD __uninitialized_copy
// (0x0045BCC1) out of line, and zero-fills then copies the __true_type tag
// temporary (`stosb`) before passing it; our /Os build expands _M_clear and
// passes the tag without a store (the tags carry an empty constructor, which
// the /O2 landings need). Tried: /Os with the STLport body, the POD trait in
// _M_allocate_and_copy.
#include "fable_stlport_vector_assign.h"

template _STL::vector<unsigned long>& _STL::vector<unsigned long>::operator=(const _STL::vector<unsigned long>& __x);
