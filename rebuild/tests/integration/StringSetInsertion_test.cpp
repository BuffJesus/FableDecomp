#include "fable_string.h"
#include "fable_stlport_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::_Rb_tree<CCharString,CCharString,_STL::_Identity<CCharString>,_STL::less<CCharString>,_STL::allocator<CCharString> > Tree;
typedef _STL::_Rb_tree_node<CCharString> Node;
static CCharStringData refs[16];static const char* texts[]={0,"\x80","\xff","","a","ab","b","z"};static int copies;
CCharString::CCharString(const CCharString& x){storage_=x.storage_;if(storage_)++storage_->owners;++copies;}
CCharString::~CCharString(){if(storage_)--storage_->owners;storage_=0;}
static int id(const Node* p){CCharStringData* ref=*(CCharStringData* const*)&p->_M_value_field;return ref?(int)(ref-refs)%8:0;}
static bool seen[8];static int previous,nodes,failed;
static int check(Node* p,Node* parent){if(!p)return 1;if(p->_M_parent!=parent){failed=1;return 0;}int l=check((Node*)p->_M_left,p);int k=id(p);if(k<=previous||!seen[k])failed=1;previous=k;++nodes;if(!p->_M_color&&((p->_M_left&&!p->_M_left->_M_color)||(p->_M_right&&!p->_M_right->_M_color)))failed=1;int r=check((Node*)p->_M_right,p);if(l!=r)failed=1;return l+(p->_M_color?1:0);}
static void release(Node* p){if(!p)return;release((Node*)p->_M_left);release((Node*)p->_M_right);p->_M_value_field.~CCharString();free(p);}
int main(){int cases=0;int zig[]={0,7,1,6,2,5,3,4},mix[]={3,1,6,0,7,2,5,4};
 for(int count=0;count<=16;++count)for(int pattern=0;pattern<4;++pattern)for(int rotation=0;rotation<8;++rotation)for(int alias=0;alias<2;++alias){
  union{double align;char data[12];} mem;memset(&mem,0,sizeof(mem));Tree* tree=(Tree*)&mem;_STL::_Rb_tree_node_base header;memset(&header,0,sizeof(header));header._M_left=header._M_right=&header;tree->_M_header._M_data=(Node*)&header;
  for(int k=0;k<16;++k){refs[k].Data.PData=(char*)texts[k%8];refs[k].owners=1;}memset(seen,0,sizeof(seen));int expected=0;copies=0;
  for(int i=0;i<count;++i){int key=((pattern==0?i%8:pattern==1?7-i%8:pattern==2?zig[i%8]:mix[i%8])+rotation)%8;void* value=key?&refs[key+(alias&&(i&1)?8:0)]:0;bool fresh=!seen[key];int before=copies;
   _STL::pair<Tree::iterator,bool> result=tree->insert_unique(*(CCharString*)&value);
   if(result.second!=fresh||id((Node*)result.first._M_node)!=key||copies-before!=(fresh?1:0))return 1;
   seen[key]=true;if(fresh)++expected;if(tree->_M_node_count!=expected)return 2;
   Node* root=(Node*)header._M_parent;if(!root||!root->_M_color||root->_M_parent!=&header)return 3;
   previous=-1;nodes=failed=0;check(root,(Node*)&header);if(failed||nodes!=expected)return 4;
   Node* left=root;while(left->_M_left)left=(Node*)left->_M_left;Node* right=root;while(right->_M_right)right=(Node*)right->_M_right;if(header._M_left!=left||header._M_right!=right)return 5;
  }
  release((Node*)header._M_parent);for(int k=0;k<16;++k)if(refs[k].owners!=1)return 6;++cases;
 }
 printf("STRING_SET_INSERTION_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
