#include "fable_game_text_bank.h"
#include "fable_stlport_tree.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::pair<const CCharString,unsigned long> Pair;
typedef _STL::_Rb_tree<CCharString,Pair,_STL::_Select1st<Pair>,_STL::less<CCharString>,_STL::allocator<Pair> > Tree;
typedef _STL::_Rb_tree_node<Pair> Node;
static CCharStringData* refs[16];static CCharStringData* anchors[16];static const char* texts[]={0,"\x80","\xff","","a","ab","b","z"};
fable_i32 g_CCharStringInstanceCount_013BD800=0;
static const char* sourceTexts[]={0,"\x80","\xff","","A","aB","B","Z"};
static const char* inputText(int i){return (i<8?sourceTexts:texts)[i%8];}
static int allocated,released,conversions,lowerings;static unsigned long pendingPayload,payloads[32];static Node* blocks[32];static bool visited[32];
static void require(bool value){if(!value){fprintf(stderr,"INIT_LOWER_FAILURE\n");exit(80);}}
struct Allocation {unsigned char* p;size_t size;bool array;bool released;};
static Allocation allocations[96];static unsigned allocationCount,scalarDeletes,arrayDeletes;
static void* allocate(size_t size,bool array){require(allocationCount<96);unsigned char* base=(unsigned char*)malloc(size+16);require(base!=0);memset(base,0x7d,size+16);Allocation& a=allocations[allocationCount++];a.p=base+8;a.size=size;a.array=array;a.released=false;return a.p;}
static void dispose(void* p,bool array){if(!p)return;Allocation* found=0;for(unsigned i=0;i<allocationCount;++i)if(allocations[i].p==p)found=&allocations[i];require(found&&!found->released&&found->array==array);for(unsigned i=0;i<8;++i)require(found->p[i-8]==0x7d&&found->p[found->size+i]==0x7d);
if(!array){CCharStringData* data=(CCharStringData*)p;require(found->size==sizeof(CCharStringData)&&data->Data.PData==0&&data->Data.StringLength==0&&data->Data.DataLength==0&&data->owners==0&&data->Data.UseFastExtend&&*((unsigned char*)p+12)==0x7d);bool anchor=false;for(int k=0;k<16;++k)if(refs[k]==p)anchor=true;require(((*(unsigned*)((char*)p+8)&0x80000000UL)!=0)==anchor);++scalarDeletes;}else ++arrayDeletes;
found->released=true;free(found->p-8);}
void* __cdecl operator new(size_t size){return allocate(size,false);}
void* __cdecl operator new[](size_t size){return allocate(size,true);}
void __cdecl operator delete(void* p){dispose(p,false);}
void __cdecl operator delete[](void* p){dispose(p,true);}
extern "C" void* __cdecl MultimapMalloc(size_t size){require(size==24&&allocated<32);unsigned char* p=(unsigned char*)malloc(size+16);require(p!=0);memset(p,0x6b,size+16);memset(p+8,0xcc,size);payloads[allocated]=pendingPayload;blocks[allocated++]=(Node*)(p+8);return p+8;}
static void guard(Node* p){unsigned char* q=(unsigned char*)p;for(int i=0;i<8;++i)require(q[i-8]==0x6b&&q[i+24]==0x6b);}
CCharString CBankFile::ConvertPathToFilename(const CCharString& filename) const {++conversions;return filename;}
char g_FableEmptyNarrowString_0129AAF4[1]={0};
fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(fable_u32,const void*,fable_u32){require(false);return 0;}
template<> unsigned long& _STL::map<CCharString,unsigned long>::operator[](const CCharString&){require(false);static unsigned long result;return result;}
extern "C" void __fastcall CallInit(CBankFile*,void*,unsigned long,const CCharString&,const CCharString&);
#pragma comment(linker,"/alternatename:@CallInit@20=?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z")
static int id(const Node* p){CCharStringData* ref=*(CCharStringData* const*)&p->_M_value_field.first;if(!ref)return 0;for(int i=0;i<8;++i)if(texts[i]&&strcmp(ref->Data.PData,texts[i])==0)return i;require(false);return 0;}
static int previous,nodes;static unsigned long previousPayload;
static int check(Node* p,Node* parent){
 if(!p)return 1;int position=-1;for(int i=0;i<allocated;++i)if(blocks[i]==p)position=i;require(position>=0&&!visited[position]);visited[position]=true;guard(p);require(*(unsigned char*)p<=1&&p->_M_parent==parent&&p->_M_value_field.second==payloads[position]);
 int l=check((Node*)p->_M_left,p);int key=id(p);require(key>=previous);if(key==previous)require(previousPayload>p->_M_value_field.second);previous=key;previousPayload=p->_M_value_field.second;++nodes;
 if(!p->_M_color)require((!p->_M_left||p->_M_left->_M_color)&&(!p->_M_right||p->_M_right->_M_color));int r=check((Node*)p->_M_right,p);require(l==r);return l+(p->_M_color?1:0);
}
static void release(Node* p){if(!p)return;release((Node*)p->_M_left);release((Node*)p->_M_right);guard(p);p->_M_value_field.~Pair();free((unsigned char*)p-8);++released;}
int main(){int cases=0;int zig[]={0,7,1,6,2,5,3,4},mix[]={3,1,6,0,7,2,5,4};
 for(int count=0;count<=24;++count)for(int pattern=0;pattern<4;++pattern)for(int rotation=0;rotation<8;++rotation)for(int alias=0;alias<2;++alias){
  union{double align;char data[sizeof(CBankFile)];} mem;memset(&mem,0,sizeof(mem));CBankFile* bank=(CBankFile*)&mem;bank->OpenFlags=128;Tree* tree=&bank->FilenameIndices._M_t;_STL::_Rb_tree_node_base header;memset(&header,0,sizeof(header));header._M_left=header._M_right=&header;tree->_M_header._M_data=(Node*)&header;
  require(g_CCharStringInstanceCount_013BD800==0);allocationCount=scalarDeletes=arrayDeletes=0;
  for(int k=0;k<16;++k){const char* text=inputText(k)?inputText(k):"";size_t length=strlen(text);refs[k]=new CCharStringData;refs[k]->Data.PData=new char[length+1];memcpy(refs[k]->Data.PData,text,length+1);refs[k]->Data.StringLength=length;*(unsigned*)((char*)refs[k]+8)=0x80000000UL|(length+1);refs[k]->Data.UseFastExtend=true;refs[k]->owners=1;anchors[k]=refs[k];++g_CCharStringInstanceCount_013BD800;}
  allocated=released=conversions=lowerings=0;int expected=0;int owners[16];for(int k=0;k<16;++k)owners[k]=1;
  for(int i=0;i<count;++i){int key=((pattern==0?i%8:pattern==1?7-i%8:pattern==2?zig[i%8]:mix[i%8])+rotation)%8;int storageKey=key+(alias&&(i&1)?8:0);void* value=key?refs[storageKey]:0;unsigned data[2]={(unsigned)value,0xffffffffUL-i};
   pendingPayload=0xffffffffUL-i;CallInit(bank,0,pendingPayload,*(CCharString*)&value,*(CCharString*)&value);
   bool inserted=key!=0&&key!=3;if(inserted){++expected;}
   require(allocated==expected&&g_CCharStringInstanceCount_013BD800==16+expected&&conversions==expected&&allocationCount==32+2*expected&&tree->_M_node_count==expected);if(inserted)require(id(blocks[expected-1])==key);
   for(int j=0;j<16;++j)require(refs[j]->owners==owners[j]&&strcmp(refs[j]->Data.PData,inputText(j)?inputText(j):"")==0);
   Node* root=(Node*)header._M_parent;if(!expected){require(!root);continue;}require(root&&root->_M_color&&root->_M_parent==&header);
   previous=-1;nodes=0;memset(visited,0,sizeof(visited));check(root,(Node*)&header);require(nodes==expected);
   Node* left=root;while(left->_M_left)left=(Node*)left->_M_left;Node* right=root;while(right->_M_right)right=(Node*)right->_M_right;require(header._M_left==left&&header._M_right==right);
  }
  release((Node*)header._M_parent);require(released==allocated&&g_CCharStringInstanceCount_013BD800==16);for(int k=0;k<16;++k)require(refs[k]->owners==1);for(int k=0;k<16;++k){((CCharString*)&anchors[k])->~CCharString();require(anchors[k]==0);}require(g_CCharStringInstanceCount_013BD800==0&&scalarDeletes==16+expected&&arrayDeletes==16+expected);for(unsigned k=0;k<allocationCount;++k)require(allocations[k].released);++cases;
 }
 printf("NATIVE_INIT_LOWER_PASS cases=%d\n",cases);return 0;
}
