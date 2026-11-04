#include <new>
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_vector_assign.h"
#include "fable_stlport_unguarded_insertion_sort.h"
typedef _STL::pair<unsigned long,CDefinitionManager::CDefClassInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefinitionManager::CDefClassInfo> Less;
template void FABLE_FASTCALL _STL::__unguarded_insertion_sort_aux(Entry*,Entry*,Entry*,Less);
