#include "test_support/string_vector_fixture.h"
#include "fable_console.h"
typedef _STL::map<CCharString,CCountedPointer<CConsoleInputBase> > Map;
typedef Map::_Rep_type Tree;
typedef Map::value_type Value;
typedef _STL::_Rb_tree_node_base Base;
typedef _STL::_Rb_tree_node<Value> Node;
static int releases;static void FABLE_FASTCALL release(void* p){if(p==(void*)0x12340000)++releases;}
static void destroyNode(Node* n){n->_M_value_field.~Value();free(n);}
static bool run(bool populated,const char* key,bool forceLeft,bool forceRight,bool wantLeft){
 Tree t;Base* h=t._M_header._M_data;Node* parent=0;
 CCountedPointer<CConsoleInputBase> owner;owner.Data=(CConsoleInputBase*)0x12340000;owner.Info=new CCPPointerInfo(release,owner.Data);
 if(populated){
  CCharString rootKey("d");Value rootValue(rootKey,owner);parent=(Node*)malloc(sizeof(Node));new(&parent->_M_value_field)Value(rootValue);
  parent->_M_color=_STL::_S_rb_tree_black;parent->_M_parent=h;parent->_M_left=parent->_M_right=0;h->_M_parent=h->_M_left=h->_M_right=parent;t._M_node_count=1;
 }
 bool ok;
 {
  CCharString text(key);Value value(text,owner);unsigned before=owner.Info->RefCount;
  Base* y=populated?(Base*)parent:h;Base* x=forceLeft?y:0;Base* w=forceRight?y:0;
  Tree::iterator result=t._M_insert(x,y,value,w);Node* n=(Node*)result._M_node;
  ok=n&&n->_M_parent==y&&!n->_M_left&&!n->_M_right&&t._M_node_count==(populated?2:1)&&owner.Info->RefCount==before+1;
  ok=ok&&n->_M_value_field.second.Info==owner.Info&&n->_M_value_field.second.Data==owner.Data&&!strcmp(n->_M_value_field.first.GetConstCharPtr(),key);
  if(!populated)ok=ok&&h->_M_parent==n&&h->_M_left==n&&h->_M_right==n&&n->_M_color==_STL::_S_rb_tree_black;
  else if(wantLeft)ok=ok&&y->_M_left==n&&h->_M_left==n&&h->_M_right==y&&n->_M_color==_STL::_S_rb_tree_red;
  else ok=ok&&y->_M_right==n&&h->_M_right==n&&h->_M_left==y&&n->_M_color==_STL::_S_rb_tree_red;
  destroyNode(n);if(owner.Info->RefCount!=before)ok=false;
 }
 if(parent)destroyNode(parent);free(h);return ok;
}
int main(){
 if(!run(false,"a",false,false,true)||!run(true,"b",false,false,true)||!run(true,"f",false,false,false)||!run(true,"f",true,false,true)||!run(true,"b",false,true,false))return 1;
 if(releases!=5||live||dataLive)return 2;
 puts("INPUT_LINK_PASS");return 0;
}