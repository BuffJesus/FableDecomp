#include "test_support/console_member_stubs.h"
CConsole::CConsole(){}
class CConsoleTestAccess {
public:
 static bool run(unsigned count,long lines){
  CConsole c;_STL::_List_node_base nodes[12];_STL::_List_node_base* h=c.CommandLineHistory._M_node;
  for(unsigned i=0;i<count;++i){nodes[i]._M_prev=i?&nodes[i-1]:h;nodes[i]._M_next=i+1<count?&nodes[i+1]:h;}
  h->_M_next=count?nodes:h;h->_M_prev=count?&nodes[count-1]:h;
  c.NumberOfHistoryLinesOnscreen=lines;c.VisibleHistoryStart._M_node=0;c.VisibleHistoryFinish._M_node=0;c.HistoryMode=true;
  c.ScrollToHistoryBeginning();
  unsigned visible=count>(unsigned long)lines?(unsigned long)lines:count;
  return c.VisibleHistoryStart._M_node==(count?nodes:h)&&c.VisibleHistoryFinish._M_node==(visible<count?&nodes[visible]:h)&&c.NumberOfHistoryLinesOnscreen==lines&&c.HistoryMode;
 }
};
int main(){const unsigned counts[]={0,1,3,9,10,12};const long lines[]={0,1,9,15,-1};
 for(unsigned i=0;i<6;++i)for(unsigned j=0;j<5;++j)if(!CConsoleTestAccess::run(counts[i],lines[j]))return 1;
 if(live||dataLive)return 2;puts("HISTORY_SCROLL_PASS");return 0;
}