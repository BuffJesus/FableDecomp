#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
typedef _STL::map<CCharString,unsigned long> Map;
typedef Map::_Rep_type Tree;typedef Map::value_type Value;typedef _STL::_Rb_tree_node<Value> Node;
struct Ord{const char* text;};
CCharString::CCharString(){storage_=0;}CCharString::~CCharString(){}
static void key(const CCharString& s,Ord* d){*(Ord**)&s=d;}
static Node nodes[8],header;
static Node* build(int lo,int hi,int shape,Node* parent){if(lo==hi)return 0;int i=shape==0?(lo+hi)/2:shape==1?lo:hi-1;Node* p=&nodes[i];p->_M_parent=parent;p->_M_left=build(lo,i,shape,p);p->_M_right=build(i+1,hi,shape,p);return p;}
int main(){static union{double align;char data[0x110];} bankStorage;CBankFile* bank=(CBankFile*)&bankStorage;Tree& tree=bank->SymbolIndices._M_t;tree._M_header._M_data=&header;
 static Ord data[]={{0},{"\x80"},{"\xff"},{""},{"a"},{"ab"},{"b"},{"z"}},copies[8],missing[]={{"c"},{"zz"}};
 for(int i=0;i<8;++i){copies[i]=data[i];key(nodes[i]._M_value_field.first,i?&data[i]:0);}
 int cases=0;CCharString query;
 for(int n=0;n<=8;++n)for(int shape=0;shape<3;++shape)for(int q=0;q<18;++q)for(int v=0;v<3;++v){
  header._M_parent=build(0,n,shape,&header);tree._M_node_count=n;
  for(int j=0;j<8;++j)nodes[j]._M_value_field.second=v==0?0:v==1?j*17+3:0xffffffff-j;
  int wanted=q<16?q%8:-1;Ord* d=q<8?&data[q]:q<16?&copies[q-8]:&missing[q-16];key(query,wanted==0?0:d);
  Node* expected=wanted>=0&&wanted<n?&nodes[wanted]:&header;
  if(tree._M_find(query)!=expected)return 1;
  unsigned long value=expected==&header?0:expected->_M_value_field.second;
  if(bank->FindIndexBySymbol(query)!=value)return 2;
  ++cases;
 }
 printf("BANK_SYMBOL_INDEX_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
