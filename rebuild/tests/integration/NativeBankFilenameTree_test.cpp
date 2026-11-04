#include "fable_string.h"
#include "fable_stlport_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::pair<const CCharString,unsigned long> Pair;
typedef _STL::_Rb_tree<CCharString,Pair,_STL::_Select1st<Pair>,_STL::less<CCharString>,_STL::allocator<Pair> > Tree;
typedef _STL::_Rb_tree_node<Pair> Node;
static CCharStringData refs[16];static const char* texts[]={0,"\x80","\xff","","a","ab","b","z"};
static int copies,allocated,released;static Node* blocks[32];static bool visited[32];
static void require(bool value){if(!value){fprintf(stderr,"MULTIMAP_FAILURE\n");exit(80);}}
extern "C" void* __cdecl MultimapMalloc(size_t size){require(size==24&&allocated<32);unsigned char* p=(unsigned char*)malloc(size+16);require(p!=0);memset(p,0x6b,size+16);memset(p+8,0xcc,size);blocks[allocated++]=(Node*)(p+8);return p+8;}
static void guard(Node* p){unsigned char* q=(unsigned char*)p;for(int i=0;i<8;++i)require(q[i-8]==0x6b&&q[i+24]==0x6b);}
CCharString::CCharString(const CCharString& x){storage_=x.storage_;if(storage_)++storage_->owners;++copies;}
CCharString::~CCharString(){if(storage_)--storage_->owners;storage_=0;}
static int id(const Node* p){CCharStringData* ref=*(CCharStringData* const*)&p->_M_value_field.first;return ref?(int)(ref-refs)%8:0;}
static int previous,nodes;static unsigned long previousPayload;
static int check(Node* p,Node* parent){
 if(!p)return 1;int position=-1;for(int i=0;i<allocated;++i)if(blocks[i]==p)position=i;require(position>=0&&!visited[position]);visited[position]=true;guard(p);require(*(unsigned char*)p<=1&&p->_M_parent==parent&&p->_M_value_field.second==0xffffffffUL-position);
 int l=check((Node*)p->_M_left,p);int key=id(p);require(key>=previous);if(key==previous)require(previousPayload>p->_M_value_field.second);previous=key;previousPayload=p->_M_value_field.second;++nodes;
 if(!p->_M_color)require((!p->_M_left||p->_M_left->_M_color)&&(!p->_M_right||p->_M_right->_M_color));int r=check((Node*)p->_M_right,p);require(l==r);return l+(p->_M_color?1:0);
}
static void release(Node* p){if(!p)return;release((Node*)p->_M_left);release((Node*)p->_M_right);guard(p);p->_M_value_field.~Pair();free((unsigned char*)p-8);++released;}
int main(){int cases=0;int zig[]={0,7,1,6,2,5,3,4},mix[]={3,1,6,0,7,2,5,4};
 for(int count=0;count<=24;++count)for(int pattern=0;pattern<4;++pattern)for(int rotation=0;rotation<8;++rotation)for(int alias=0;alias<2;++alias){
  union{double align;char data[12];} mem;memset(&mem,0,sizeof(mem));Tree* tree=(Tree*)&mem;_STL::_Rb_tree_node_base header;memset(&header,0,sizeof(header));header._M_left=header._M_right=&header;tree->_M_header._M_data=(Node*)&header;
  for(int k=0;k<16;++k){refs[k].Data.PData=(char*)texts[k%8];refs[k].owners=1;}copies=allocated=released=0;int owners[16];for(int k=0;k<16;++k)owners[k]=1;
  for(int i=0;i<count;++i){int key=((pattern==0?i%8:pattern==1?7-i%8:pattern==2?zig[i%8]:mix[i%8])+rotation)%8;int storageKey=key+(alias&&(i&1)?8:0);void* value=key?&refs[storageKey]:0;unsigned data[2]={(unsigned)value,0xffffffffUL-i};
   Tree::iterator result=tree->insert_equal(*(Pair*)data);
   require(allocated==i+1&&copies==i+1&&result._M_node==blocks[i]&&id(blocks[i])==key&&tree->_M_node_count==i+1);
   if(key)++owners[storageKey];for(int j=0;j<16;++j)require(refs[j].owners==owners[j]);
   Node* root=(Node*)header._M_parent;require(root&&root->_M_color&&root->_M_parent==&header);
   previous=-1;nodes=0;memset(visited,0,sizeof(visited));check(root,(Node*)&header);require(nodes==i+1);
   Node* left=root;while(left->_M_left)left=(Node*)left->_M_left;Node* right=root;while(right->_M_right)right=(Node*)right->_M_right;require(header._M_left==left&&header._M_right==right);
  }
  release((Node*)header._M_parent);require(released==allocated);for(int k=0;k<16;++k)require(refs[k].owners==1);++cases;
 }
 printf("NATIVE_MULTIMAP_PASS cases=%d\n",cases);return 0;
}
