#include "fable_console.h"
void CConsole::AddCommandLineToHistory(const CConsoleCommandLine& line){
 if(CommandLineHistory.size()>=256)CommandLineHistory.pop_back();
 CommandLineHistory.push_front(line);
 ScrollToHistoryBeginning();
}