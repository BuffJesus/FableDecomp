// _STL::__unguarded_partition<pair<unsigned long, CDefClassBase::CSubDefInfo>*, ...> @ 0x009B3800, from
// __introsort_loop 0x009B7DD0.
#include "fable_def_class_base.h"
#include "fable_stlport_partition.h"

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> SubDefEntry;
typedef CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> SubDefLess;

template SubDefEntry* FABLE_FASTCALL _STL::__unguarded_partition(SubDefEntry* __first, SubDefEntry* __last, SubDefEntry __pivot, SubDefLess __comp);
