// _STL::sort<pair<unsigned long, CDefClassBase::CSubDefInfo>*, CKeyPairCompareLess<...> > @ 0x009B84C0
// (from CVectorMap::Sort in CParentDefClassBase::AddSubDef 0x009B00D0 and
// InternalSaveBinary 0x009B0200). This unit also emits the out-of-line final
// insertion pass __final_insertion_sort 0x009B6830 (body in fable_stlport_algo.h).
#include "fable_def_class_base.h"
#include "fable_stlport_algo_impl.h"

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> SubDefEntry;
typedef CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> SubDefLess;

template void FABLE_FASTCALL _STL::sort(SubDefEntry* __first, SubDefEntry* __last, SubDefLess __comp);
