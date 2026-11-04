#include "test_support/string_vector_fixture.h"
#include "fable_console.h"
static int baseCtor, baseDtor;
CBaseClass::CBaseClass(){++baseCtor;}
CBaseClass::~CBaseClass(){++baseDtor;}
namespace _STL {template<class T,class A> list<T,A>::~list(){free(_M_node);}}
class CConsoleTestAccess {
public:
 template<class M> static bool empty(M& m){
  _STL::_Rb_tree_node_base* h=m._M_t._M_header._M_data;
  return h&&!h->_M_color&&!h->_M_parent&&h->_M_left==h&&h->_M_right==h&&!m._M_t._M_node_count;
 }
 static void freeMaps(CConsole& c){free(c.KeyBindingMap._M_t._M_header._M_data);free(c.StringBindingMap._M_t._M_header._M_data);free(c.RegisteredConsoleInputs._M_t._M_header._M_data);}
 static int check(CConsole& c){
  if(!empty(c.KeyBindingMap)||!empty(c.StringBindingMap)||!empty(c.RegisteredConsoleInputs))return 1;
  if(c.KeyBindingMap._M_t._M_header._M_data==(void*)c.StringBindingMap._M_t._M_header._M_data)return 2;
  if(c.FrontCommandLine.UserEntry||c.BackCommandLine.UserEntry||c.FrontCommandLine.CursorPosition||c.BackCommandLine.CursorPosition)return 3;
  if(strcmp(c.FrontCommandLine.CurrentEntry.GetConstCharPtr(),"")||strcmp(c.BackCommandLine.CurrentEntry.GetConstCharPtr(),"")||strcmp(c.HelpBarString.GetConstCharPtr(),""))return 4;
  _STL::_List_node_base* h=c.CommandLineHistory._M_node;
  if(!h||h->_M_next!=h||h->_M_prev!=h||c.VisibleHistoryStart._M_node!=h||c.VisibleHistoryFinish._M_node!=h)return 5;
  if(c.NumberOfHistoryLinesOnscreen!=9||c.HistoryMode||!c.MovingIntoHistory||c.GuessMode||c.GuessPos._M_node||c.PFont||c.Active||c.ConsoleAlpha!=255||c.CursorBlink||c.CursorBlinkRate!=32)return 6;
  if(c.HistoryWindowColour.GetD3DCOLOR()!=0xff3c3c3c||c.ClientWindowColour.GetD3DCOLOR()!=0xff000000||c.HelpWindowColour.GetD3DCOLOR()!=0xff3c3c3c||c.ScrollBarColour.GetD3DCOLOR()!=0xff000000||c.TextColour.GetD3DCOLOR()!=0xffffffff)return 7;
  if((unsigned long)c.HistoryPos._M_node!=0xa5a5a5a5||c.ToggleChar!=(char)0xa5||(unsigned long)c.ToggleKey!=0xa5a5a5a5)return 8;
  return 0;
 }
};
CConsole::~CConsole(){CConsoleTestAccess::freeMaps(*this);}
int main(){
 double storage[21];memset(storage,0xa5,sizeof(storage));
 CConsole* c=new(storage)CConsole;int result=CConsoleTestAccess::check(*c);if(result)return result;
 if(baseCtor!=2||live!=3)return 9;
 for(unsigned i=sizeof(CConsole);i<sizeof(storage);++i)if(((unsigned char*)storage)[i]!=0xa5)return 10;
 c->~CConsole();if(live||dataLive||baseDtor!=2)return 11;
 puts("CONSOLE_CTOR_PASS");return 0;
}
