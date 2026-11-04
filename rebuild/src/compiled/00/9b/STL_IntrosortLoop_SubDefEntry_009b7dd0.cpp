// _STL::__introsort_loop<pair<unsigned long, CDefClassBase::CSubDefInfo>*, ...> @ 0x009B7DD0, from sort
// 0x009B84C0; partitions with __unguarded_partition 0x009B3800 and falls back
// to __partial_sort 0x009B6AD0.
#include "fable_def_class_base.h"
#include "fable_stlport_introsort.h"

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> SubDefEntry;
typedef CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> SubDefLess;

template void FABLE_FASTCALL _STL::__introsort_loop(SubDefEntry* __first, SubDefEntry* __last, SubDefEntry*, int __depth_limit, SubDefLess __comp);
