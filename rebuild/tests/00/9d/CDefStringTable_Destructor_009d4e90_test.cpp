#include "fable_def_string.h"
#include "test_support/base_class_stubs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <new>

typedef _STL::map<CCharString,long> Map;
typedef _STL::_Rb_tree_node<Map::value_type> Node;
static CDefStringTable* table;static char events[32];static unsigned count;
static bool addDuringStream;
static void check(bool ok){if(!ok)abort();}
static void record(char c){events[count++]=c;}
static Node* node(char id){Node* n=(Node*)calloc(1,sizeof(Node));*reinterpret_cast<void**>(const_cast<CCharString*>(&n->_M_value_field.first))=reinterpret_cast<void*>(id);return n;}
CCharString::~CCharString(){if(this==&table->StringStream.NewLine)record('N');else record(static_cast<char>(reinterpret_cast<unsigned long>(*reinterpret_cast<void**>(this))));}
CWideString::~CWideString(){check(this==&table->TableFilename);check(table->StringMap._M_t.size()==0);record('F');}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){record('B');}
CMemoryDataOutputStream::~CMemoryDataOutputStream(){
    check(this==&table->StringStream);check(table->StringMap._M_t.size()==0);
    _STL::_Rb_tree_node_base* h=table->StringMap._M_t._M_header._M_data;
    check(h&&h->_M_parent==0&&h->_M_left==h&&h->_M_right==h);record('M');
    if(addDuringStream){Node* n=node('X');n->_M_parent=h;h->_M_parent=n;h->_M_left=n;h->_M_right=n;table->StringMap._M_t._M_node_count=1;}
}
void CMemoryDataOutputStream::SetPosition(unsigned long){}
unsigned long CMemoryDataOutputStream::GetPosition()const{return 0;}
void CMemoryDataOutputStream::Close(){}
void CMemoryDataOutputStream::Write(const void*,long){}
static void run(unsigned nodes,bool reentrant){
    unsigned char storage[sizeof(CDefStringTable)];memset(storage,0,sizeof(storage));table=reinterpret_cast<CDefStringTable*>(storage);
    Node* h=(Node*)calloc(1,sizeof(Node));h->_M_left=h;h->_M_right=h;table->StringMap._M_t._M_header._M_data=h;
    Node* root=0;Node* left=0;Node* right=0;
    if(nodes){root=node('R');root->_M_parent=h;h->_M_parent=root;h->_M_left=root;h->_M_right=root;}
    if(nodes>1){left=node('L');left->_M_parent=root;root->_M_left=left;right=node('T');right->_M_parent=root;root->_M_right=right;h->_M_left=left;h->_M_right=right;}
    table->StringMap._M_t._M_node_count=nodes;count=0;addDuringStream=reentrant;
    table->~CDefStringTable();
    const char* prefix=nodes==0?"":nodes==1?"R":"TRL";
    unsigned n=strlen(prefix);check(count==n+4+(reentrant?1:0)&&!memcmp(events,prefix,n)&&!memcmp(events+n,"FMNB",4));
    if(reentrant)check(events[n+4]=='X');
}
int main(){run(0,false);run(1,false);run(3,false);run(3,true);puts("DEFSTRING_DTOR_OK cases=4");return 0;}
