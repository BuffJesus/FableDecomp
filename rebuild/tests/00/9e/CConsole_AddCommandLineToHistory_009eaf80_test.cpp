#include "test_support/console_member_stubs.h"
static int removed,lastRemoved,insertions,scrolls;static bool valid;
static const CConsoleCommandLine* expectedLine;static void* expectedList;
CConsole::CConsole(){}
struct OldLine:CConsoleCommandLine{virtual ~OldLine(){++removed;lastRemoved=CursorPosition;}};
namespace _STL {
 template<> list<CConsoleCommandLine>::iterator list<CConsoleCommandLine>::insert(iterator position,const CConsoleCommandLine& value){
  ++insertions;valid=valid&&this==expectedList&&position._M_node==_M_node->_M_next&&&value==expectedLine;
  _Node* n=(_Node*)malloc(sizeof(_Node));new(&n->_M_data)CConsoleCommandLine(value);n->_M_next=position._M_node;n->_M_prev=position._M_node->_M_prev;n->_M_prev->_M_next=n;position._M_node->_M_prev=n;return iterator(n);
 }
}
void CConsole::ScrollToHistoryBeginning(){++scrolls;valid=valid&&insertions==1;}
class CConsoleTestAccess{
public:
 static _STL::list<CConsoleCommandLine>& list(CConsole& c){return c.CommandLineHistory;}
 static void add(CConsole& c,const CConsoleCommandLine& line){c.AddCommandLineToHistory(line);}
};
int main(){
 const unsigned counts[]={0,255,256,257};
 for(unsigned k=0;k<4;++k){CConsole c;_STL::list<CConsoleCommandLine>& list=CConsoleTestAccess::list(c);expectedList=&list;
  for(unsigned i=0;i<counts[k];++i){_STL::list<CConsoleCommandLine>::_Node* n=(_STL::list<CConsoleCommandLine>::_Node*)malloc(sizeof(*n));new(&n->_M_data)OldLine;n->_M_data.CursorPosition=i;n->_M_next=list._M_node;n->_M_prev=list._M_node->_M_prev;n->_M_prev->_M_next=n;list._M_node->_M_prev=n;}
  CConsoleCommandLine line(CCharString("new message"));line.UserEntry=true;line.CursorPosition=42;expectedLine=&line;
  removed=lastRemoved=insertions=scrolls=0;valid=true;CConsoleTestAccess::add(c,line);
  unsigned expected=counts[k]+1-(counts[k]>=256);
  if(!valid||list.size()!=expected||insertions!=1||scrolls!=1||removed!=(counts[k]>=256?1:0)||(removed&&lastRemoved!=(int)counts[k]-1))return 1;
  CConsoleCommandLine& front=((_STL::list<CConsoleCommandLine>::_Node*)list._M_node->_M_next)->_M_data;
  if(!front.UserEntry||front.CursorPosition!=42||strcmp(front.CurrentEntry.GetConstCharPtr(),"new message"))return 2;
  while(list._M_node->_M_next!=list._M_node){_STL::list<CConsoleCommandLine>::_Node* n=(_STL::list<CConsoleCommandLine>::_Node*)list._M_node->_M_next;list._M_node->_M_next=n->_M_next;n->_M_data.~CConsoleCommandLine();free(n);}
 }
 if(live||dataLive)return 3;puts("HISTORY_ADD_PASS");return 0;
}