#include "fable_console.h"

template<>
void CConsoleCommand<CConsole>::CallCommandFunction(const CGenericVarVector& params) const
{
    CommandFunc(Receiver, params);
}
