// CConsole::ProcessBindKeyInput @ 0x009EA540 (FableWin
// ?ProcessBindKeyInput@CConsole@@QAE_NABVCInputEvent@@@Z). Retail compiles the
// key bindings out: CInputProcessConsole::ProcessInput 0x00687F80 calls this at
// 0x00688106 (after CConsole::Get) while the console is inactive and skips its
// handling on false.
#include "fable_console.h"

bool CConsole::ProcessBindKeyInput(const CInputEvent&)
{
    return false;
}
