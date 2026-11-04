#include "fable_console.h"
void CConsole::SendMessageToUser(const CCharString& message) {
 AddCommandLineToHistory(CConsoleCommandLine(message));
}