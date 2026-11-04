#include "fable_console.h"
void CConsole::ScrollToHistoryBeginning(){
 VisibleHistoryStart=VisibleHistoryFinish=CommandLineHistory.begin();
 if(CommandLineHistory.size()>NumberOfHistoryLinesOnscreen)
  _STL::advance(VisibleHistoryFinish,NumberOfHistoryLinesOnscreen);
 else _STL::advance(VisibleHistoryFinish,CommandLineHistory.size());
}