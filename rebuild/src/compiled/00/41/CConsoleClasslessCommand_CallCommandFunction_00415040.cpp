#include "fable_console.h"

void CConsoleClasslessCommand::CallCommandFunction(const CGenericVarVector& params) const
{
    CommandFunc(params);
}
