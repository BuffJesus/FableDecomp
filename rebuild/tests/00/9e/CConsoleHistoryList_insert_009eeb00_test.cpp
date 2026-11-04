#include "test_support/string_vector_fixture.h"
#include "fable_console.h"
static int roots,rootDeaths;
CBaseClass::CBaseClass(){++roots;}
CBaseClass::~CBaseClass(){++rootDeaths;}
namespace _STL {template<class T,class A> list<T,A>::~list(){free(_M_node);}}
int main(){
 { _STL::list<CConsoleCommandLine> list;CConsoleCommandLine value(CCharString("message"));value.UserEntry=true;value.CursorPosition=23;
 typedef _STL::list<CConsoleCommandLine>::_Node Node;_STL::_List_node_base* h=list._M_node;
 Node* a=(Node*)list.insert(list.begin(),value)._M_node;
 Node* b=(Node*)list.insert(list.end(),value)._M_node;
 Node* c=(Node*)list.insert(_STL::list<CConsoleCommandLine>::iterator(b),value)._M_node;
 if(h->_M_next!=a||a->_M_next!=c||c->_M_next!=b||b->_M_next!=h||h->_M_prev!=b||b->_M_prev!=c||c->_M_prev!=a||a->_M_prev!=h||list.size()!=3)return 1;
 Node* nodes[]={a,b,c};for(int i=0;i<3;++i){CConsoleCommandLine& x=nodes[i]->_M_data;if(!x.UserEntry||x.CursorPosition!=23||strcmp(x.CurrentEntry.GetConstCharPtr(),"message"))return 2;}
 if(roots!=4||live!=4)return 3;
 for(int i=0;i<3;++i){nodes[i]->_M_data.~CConsoleCommandLine();free(nodes[i]);}
 }
 if(roots!=rootDeaths||live||dataLive)return 4;puts("HISTORY_INSERT_PASS");return 0;
}