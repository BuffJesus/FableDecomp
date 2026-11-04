// CConsole::Draw @ 0x009EA550 (FableWin ?Draw@CConsole@@QAEXABVC2DBoxF@@@Z).
// Retail compiles the console drawing out; the game's frame code calls it at
// 0x00435C55 with a screen box while CConsole::IsActive.
#include "fable_console.h"

void CConsole::Draw(const C2DBoxF&)
{
}
