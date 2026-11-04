// _STL::__unguarded_insertion_sort_aux<pair<unsigned long, CDefClassBase::CSubDefInfo>*, ...> @ 0x009B2CB0,
// from the final insertion pass 0x009B6830. This unit also emits
// __unguarded_linear_insert 0x009B1A30, which stays a call.
#include "fable_def_class_base.h"
#include "fable_stlport_unguarded_insertion_sort.h"

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> SubDefEntry;
typedef CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> SubDefLess;

template void FABLE_FASTCALL _STL::__unguarded_insertion_sort_aux(SubDefEntry* __first, SubDefEntry* __last, SubDefEntry*, SubDefLess __comp);
