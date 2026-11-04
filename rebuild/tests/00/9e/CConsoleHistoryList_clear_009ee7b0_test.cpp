#include "test_support/string_vector_fixture.h"
#include "fable_console.h"
static int lineDeaths,derivedDeaths;
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){++lineDeaths;}
struct DerivedLine:CConsoleCommandLine{virtual ~DerivedLine(){++derivedDeaths;}};
namespace _STL {template<class T,class A> list<T,A>::~list(){free(_M_node);}}
int main(){
 { _STL::list<CConsoleCommandLine> list;_STL::_List_node_base* h=list._M_node;
 list.clear();if(lineDeaths||h->_M_next!=h||h->_M_prev!=h)return 1;
 for(int i=0;i<3;++i){
  _STL::_List_node<CConsoleCommandLine>* n=(_STL::_List_node<CConsoleCommandLine>*)malloc(sizeof(*n));
  new(&n->_M_data)DerivedLine;n->_M_prev=h->_M_prev;n->_M_next=h;h->_M_prev->_M_next=n;h->_M_prev=n;
 }
 if(live!=3)return 2;
 list.clear();if(lineDeaths!=3||derivedDeaths!=3||list._M_node!=h||h->_M_next!=h||h->_M_prev!=h||live||dataLive)return 3;
 list.clear();if(lineDeaths!=3||derivedDeaths!=3)return 4;
 }
 puts("HISTORY_CLEAR_PASS");return 0;
}
