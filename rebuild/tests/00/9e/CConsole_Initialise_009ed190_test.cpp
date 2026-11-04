#include "test_support/console_member_stubs.h"
static int allocation, failAt=-1, inputs, clears, historyClears, messages, sequence;
static CConsole* expected;
static CConsoleInputBase* captured[15];
static const char* names[]={"BindString","RunBoundString","BindKey","RunScript","ConsoleListContaining","CommandList","CommandListContaining","VarList","VarListContaining","SetConsoleTextColour","SetConsoleHistoryWindowColour","SetConsoleClientWindowColour","SetConsoleHelpWindowColour","SetConsoleScrollBarColour","ConsoleAlpha"};
static bool valid=true;
void* operator new(size_t n){int i=allocation++;return i==failAt?0:malloc(n);}
void operator delete(void* p){free(p);}
CConsole::CConsole(){}
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType a,EGenericVarType b){
 ParamTypes._M_start=(EGenericVarType*)malloc(8);ParamTypes._M_start[0]=a;ParamTypes._M_start[1]=b;ParamTypes._M_finish=ParamTypes._M_end_of_storage._M_data=ParamTypes._M_start+2;
}
CConsoleCommandParameters::CConsoleCommandParameters(EGenericVarType a,EGenericVarType b,EGenericVarType c){
 ParamTypes._M_start=(EGenericVarType*)malloc(12);ParamTypes._M_start[0]=a;ParamTypes._M_start[1]=b;ParamTypes._M_start[2]=c;ParamTypes._M_finish=ParamTypes._M_end_of_storage._M_data=ParamTypes._M_start+3;
}
bool CConsoleCommandBase::ProcessCommandLine(const CCharString&,CConsole&){return false;}
CCharString CConsoleCommandBase::GetFormattedName()const{return CCharString();}
CCharString CConsoleCommandBase::GetTypeAsString()const{return CCharString();}
CCharString CConsoleCommandBase::GetHelp(){return CCharString();}
bool CConsoleVariable::ProcessCommandLine(const CCharString&,CConsole&){return false;}
CCharString CConsoleVariable::GetFormattedName()const{return CCharString();}
CCharString CConsoleVariable::GetTypeAsString()const{return CCharString();}
CCharString CConsoleVariable::GetHelp(){return CCharString();}
template<> void CConsoleCommand<CConsole>::CallCommandFunction(const CGenericVarVector&)const{}
typedef _STL::map<CCharString,CCountedPointer<CConsoleInputBase> > InputMap;
static _STL::_Rb_tree_node<InputMap::value_type>* seed;
namespace _STL {
 template<> void InputMap::_Rep_type::_M_erase(_Rb_tree_node<InputMap::value_type>* p){valid=valid&&p==seed&&sequence++==0;++clears;}
 template<> void list<CConsoleCommandLine>::clear(){++historyClears;valid=valid&&inputs==15;_M_node->_M_next=_M_node;_M_node->_M_prev=_M_node;}
}
class CConsoleTestAccess {
public:
 static void seedMap(CConsole& c,bool nonempty){
  c.RegisteredConsoleInputs._M_t._M_node_count=nonempty?1:0;
  c.RegisteredConsoleInputs._M_t._M_header._M_data->_M_parent=nonempty?seed:0;
 }
 static bool check(CConsole& c){return c.ToggleChar=='~'&&c.ToggleKey==KB_HASH&&c.PFont==(const CFontBank*)0x12340000&&c.HistoryPos._M_node==c.CommandLineHistory._M_node&&c.RegisteredConsoleInputs._M_t._M_node_count==0&&c.ConsoleAlpha==123;}
 static void setAlpha(CConsole& c){c.ConsoleAlpha=123;}
 static long* alpha(CConsole& c){return &c.ConsoleAlpha;}
};
class CConsoleInputTestAccess {
public:
 static bool check(CConsoleInputBase* p,int i);
};
void CConsole::AddInput(CConsoleInputBase* p){valid=valid&&this==expected&&inputs<15;if(inputs<15)captured[inputs++]=p;}
void CConsole::SendMessageToUser(const CCharString& s){++messages;valid=valid&&this==expected&&historyClears==1&&CConsoleTestAccess::check(*this)&&!strcmp(s.GetConstCharPtr(),"Try and shut up and get on with your work.");}
void FABLE_FASTCALL CConsole::ConsoleAddStringBinding(CConsole&,const CGenericVarVector&){sequence=100;}
void FABLE_FASTCALL CConsole::ConsoleRunStringBinding(CConsole&,const CGenericVarVector&){sequence=101;}
void FABLE_FASTCALL CConsole::ConsoleAddKeyBinding(CConsole&,const CGenericVarVector&){sequence=102;}
void FABLE_FASTCALL CConsole::ConsoleRunScript(CConsole&,const CGenericVarVector&){sequence=103;}
void FABLE_FASTCALL CConsole::ConsoleConsoleListContainingString(CConsole&,const CGenericVarVector&){sequence=104;}
void FABLE_FASTCALL CConsole::ConsoleCommandList(CConsole&,const CGenericVarVector&){sequence=105;}
void FABLE_FASTCALL CConsole::ConsoleCommandListContainingString(CConsole&,const CGenericVarVector&){sequence=106;}
void FABLE_FASTCALL CConsole::ConsoleVariableList(CConsole&,const CGenericVarVector&){sequence=107;}
void FABLE_FASTCALL CConsole::ConsoleVariableListContainingString(CConsole&,const CGenericVarVector&){sequence=108;}
void FABLE_FASTCALL CConsole::SetConsoleTextColour(CConsole&,const CGenericVarVector&){sequence=109;}
void FABLE_FASTCALL CConsole::SetConsoleHistoryWindowColour(CConsole&,const CGenericVarVector&){sequence=110;}
void FABLE_FASTCALL CConsole::SetConsoleClientWindowColour(CConsole&,const CGenericVarVector&){sequence=111;}
void FABLE_FASTCALL CConsole::SetConsoleHelpWindowColour(CConsole&,const CGenericVarVector&){sequence=112;}
void FABLE_FASTCALL CConsole::SetConsoleScrollBarColour(CConsole&,const CGenericVarVector&){sequence=113;}
static CConsoleCommand<CConsole>::Function functions[]={&CConsole::ConsoleAddStringBinding,&CConsole::ConsoleRunStringBinding,&CConsole::ConsoleAddKeyBinding,&CConsole::ConsoleRunScript,&CConsole::ConsoleConsoleListContainingString,&CConsole::ConsoleCommandList,&CConsole::ConsoleCommandListContainingString,&CConsole::ConsoleVariableList,&CConsole::ConsoleVariableListContainingString,&CConsole::SetConsoleTextColour,&CConsole::SetConsoleHistoryWindowColour,&CConsole::SetConsoleClientWindowColour,&CConsole::SetConsoleHelpWindowColour,&CConsole::SetConsoleScrollBarColour};
bool CConsoleInputTestAccess::check(CConsoleInputBase* p,int i){
 if(!p)return i==failAt;
 if(i==failAt||strcmp(p->Name.GetConstCharPtr(),names[i]))return false;
 if(i==14){CConsoleVariable* v=static_cast<CConsoleVariable*>(p);return v->Var.Type==GENERIC_VAR_SLONG&&v->Var.PSLong==CConsoleTestAccess::alpha(*expected);}
 CConsoleCommand<CConsole>* c=static_cast<CConsoleCommand<CConsole>*>(p);
 if(&c->Receiver!=expected||c->CommandFunc!=functions[i])return false;
 const unsigned counts[]={2,1,2,1,1,0,1,0,1,3,3,3,3,3};
 if(c->ParamTypes.ParamTypes.size()!=counts[i])return false;
 for(unsigned j=0;j<counts[i];++j)if(c->ParamTypes.ParamTypes._M_start[j]!=(i>=9?GENERIC_VAR_ULONG:GENERIC_VAR_STRING))return false;
 return true;
}
int main(){
 _STL::_Rb_tree_node_base dummy;seed=(_STL::_Rb_tree_node<InputMap::value_type>*)&dummy;
 for(int failure=-1;failure<15;++failure){
  CConsole c;expected=&c;CConsoleTestAccess::setAlpha(c);
  for(int pass=0;pass<2;++pass){
   failAt=failure;allocation=inputs=clears=historyClears=messages=sequence=0;valid=true;memset(captured,0,sizeof(captured));
   CConsoleTestAccess::seedMap(c,pass!=0);c.Initialise('~',KB_HASH,(const CFontBank*)0x12340000);
   if(!valid||allocation!=15||inputs!=15||clears!=pass||historyClears!=1||messages!=1||!CConsoleTestAccess::check(c))return 1;
   for(int i=0;i<15;++i){if(!CConsoleInputTestAccess::check(captured[i],i))return 2;delete captured[i];}
  }
 }
 if(live||dataLive)return 3;
 puts("CONSOLE_INIT_PASS");return 0;
}
