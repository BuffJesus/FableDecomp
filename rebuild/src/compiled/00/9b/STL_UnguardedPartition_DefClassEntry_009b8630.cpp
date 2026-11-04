#include <new>
#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_vector_assign.h"
#include "fable_stlport_partition.h"
typedef _STL::pair<unsigned long,CDefinitionManager::CDefClassInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefinitionManager::CDefClassInfo> Less;
template Entry* FABLE_FASTCALL _STL::__unguarded_partition(Entry*,Entry*,Entry,Less);
