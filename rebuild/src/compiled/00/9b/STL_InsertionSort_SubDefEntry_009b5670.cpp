// _STL::__insertion_sort<pair<unsigned long, CDefClassBase::CSubDefInfo>*, ...> @ 0x009B5670, from the final
// insertion pass 0x009B6830. __linear_insert expands inline for these 12-byte
// entries; this unit also emits __copy_backward 0x009B18E0, which stays a call.
#include "fable_def_class_base.h"
#include "fable_stlport_linear_insert.h"
#include "fable_stlport_insertion_sort.h"

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> SubDefEntry;
typedef CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> SubDefLess;

template void FABLE_FASTCALL _STL::__insertion_sort(SubDefEntry* __first, SubDefEntry* __last, SubDefLess __comp);
