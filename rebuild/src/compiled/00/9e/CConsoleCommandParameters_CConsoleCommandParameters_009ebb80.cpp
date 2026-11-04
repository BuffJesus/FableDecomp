#include <new>
#include "fable_console.h"
#include "fable_stlport_uninitialized.h"
#include "fable_stlport_vector_impl.h"

// lib_console2.cpp, retail 0x009EBB30-0x009EBD90: the parameter-list
// constructors (each type appended in turn) and the push_back they share.

// Retail 009EBB30.
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType type)
{
    ParamTypes.push_back(type);
}

// Retail 009EBB80.
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType first,EGenericVarType second) { ParamTypes.push_back(first); ParamTypes.push_back(second); }

// Retail 009EBCB0.
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType first, EGenericVarType second, EGenericVarType third,
                                                     EGenericVarType fourth)
{
    ParamTypes.push_back(first);
    ParamTypes.push_back(second);
    ParamTypes.push_back(third);
    ParamTypes.push_back(fourth);
}
