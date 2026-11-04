#include <ctype.h>
#include "test_support/console_member_stubs.h"
static int destroyed, insertCalls;static bool failInfo, reject, valid=true;
static CConsoleInputBase* held;static CCPPointerInfo* heldInfo;
static void* expectedTree;
void* operator new(size_t n){return failInfo&&n==sizeof(CCPPointerInfo)?0:malloc(n);}
void operator delete(void* p){free(p);}
CConsole::CConsole(){}
CCharString CCharString::GetAsLowerCase()const{
 char text[128];unsigned i=0;const char* source=GetConstCharPtr();
 for(;source[i]&&i<127;++i)text[i]=(char)tolower((unsigned char)source[i]);text[i]=0;return CCharString(text);
}
class Input : public CConsoleInputBase {
public:
 Input(const char* name):CConsoleInputBase(name){}
 virtual ~Input(){++destroyed;}
 bool ProcessCommandLine(const CCharString&,CConsole&){return false;}
 CCharString GetFormattedName()const{return CCharString();}
 CCharString GetTypeAsString()const{return CCharString();}
 CCharString GetHelp(){return CCharString();}
};
class CConsoleTestAccess {public:static void* tree(CConsole& c){return &c.RegisteredConsoleInputs._M_t;}};
typedef _STL::map<CCharString,CCountedPointer<CConsoleInputBase> > InputMap;
namespace _STL {
 template<> pair<InputMap::iterator,bool> InputMap::_Rep_type::insert_unique(const InputMap::value_type& v){
  ++insertCalls;valid=valid&&this==expectedTree&&!strcmp(v.first.GetConstCharPtr(),"mixedname");
  valid=valid&&v.second.Data&&(!v.second.Info||v.second.Info->RefCount==3);
  if(!reject){held=v.second.Data;heldInfo=v.second.Info;if(heldInfo)++heldInfo->RefCount;}
  return pair<InputMap::iterator,bool>(end(),!reject);
 }
}
static void releaseHeld(){if(heldInfo){heldInfo->DeleteFunc(heldInfo->Data);delete heldInfo;}held=0;heldInfo=0;}
int main(){
 {CConsole c;expectedTree=CConsoleTestAccess::tree(c);
 Input* a=new Input("MiXeDName");c.AddInput(a);
 if(!valid||insertCalls!=1||held!=a||!heldInfo||heldInfo->RefCount!=1||destroyed||strcmp(a->GetName().GetConstCharPtr(),"MiXeDName"))return 1;
 reject=true;Input* b=new Input("MIXEDNAME");c.AddInput(b);
 if(!valid||insertCalls!=2||held!=a||heldInfo->RefCount!=1||destroyed!=1)return 2;
 releaseHeld();if(destroyed!=2)return 3;
 Input* unowned=new Input("MixedName");failInfo=true;c.AddInput(unowned);failInfo=false;
 if(!valid||destroyed!=2||insertCalls!=3)return 4;delete unowned;
 }
 if(destroyed!=3||live||dataLive)return 5;
 puts("CONSOLE_ADD_PASS");return 0;
}