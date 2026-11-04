// CConsole::ProcessInput @ 0x009EA530 (FableWin ?ProcessInput@CConsole@@QAE_NABVCInputEvent@@@Z).
//
// The retail build compiles the console's input handling out: the event is
// never consumed (CInputProcessConsole::ProcessInput 0x00687F80 calls this
// through the console singleton and falls through on false).

#include "fable_console.h"

bool CConsole::ProcessInput(const CInputEvent&)
{
    return false;
}
