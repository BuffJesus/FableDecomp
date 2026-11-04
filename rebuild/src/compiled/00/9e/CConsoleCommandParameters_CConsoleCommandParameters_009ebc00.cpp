#include <new>
#include "fable_console.h"
#include "fable_stlport_uninitialized.h"
#include "fable_stlport_vector_impl.h"
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType first,EGenericVarType second,EGenericVarType third) { ParamTypes.push_back(first); ParamTypes.push_back(second); ParamTypes.push_back(third); }