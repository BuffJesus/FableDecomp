#include "fable_game_text_bank.h"
#include "fable_stlport_tree.h"
#include "fable_filesystem.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::pair<const CCharString,unsigned long> Pair;
typedef _STL::_Rb_tree<CCharString,Pair,_STL::_Select1st<Pair>,_STL::less<CCharString>,_STL::allocator<Pair> > Tree;
typedef _STL::_Rb_tree_node<Pair> Node;
static CCharStringData* refs[16];static CCharStringData* anchors[16];static const char* texts[]={0,"\x80","\xff","","a","ab.big","b","z"};
fable_i32 g_CCharStringInstanceCount_013BD800=0;
static const char* sourceTexts[]={0,"C:\\DIR\\\x80","/root/\xff","","D:A","folder\\aB.BIG","\\\\server\\share\\B","/root/Z"};
static const char* aliasTexts[]={0,"C:\\DIR\\\x80","/ROOT/\xff","","D:a","FOLDER\\ab.big","\\\\server\\share\\b","/ROOT/z"};
static const char* inputText(int i){return (i<8?sourceTexts:aliasTexts)[i%8];}
static int allocated,released,conversions,lowerings;static unsigned long pendingPayload,payloads[64];static Node* blocks[64];static bool visited[64];
static void require(bool value){if(!value){fprintf(stderr,"INIT_COMPLETE_FAILURE\n");exit(80);}}
struct Block {unsigned char* p;size_t size;int kind;bool freed;};
static Block allocations[1024];static unsigned allocationCount;
static void* allocate(size_t size,int kind){require(allocationCount<1024&&size<1024);unsigned char* p=(unsigned char*)malloc(size+32);require(p!=0);memset(p,0x7d,size+32);Block& b=allocations[allocationCount++];b.p=p+16;b.size=size;b.kind=kind;b.freed=false;return b.p;}
static void dispose(void* p,int kind){if(!p)return;Block* b=0;for(unsigned i=0;i<allocationCount;++i)if(allocations[i].p==p&&!allocations[i].freed)b=&allocations[i];require(b&&b->kind==kind);for(int i=0;i<16;++i)require(b->p[i-16]==0x7d&&b->p[b->size+i]==0x7d);b->freed=true;free(b->p-16);}
extern "C" void* __cdecl PathMalloc(size_t n){return allocate(n,0);}
extern "C" void __cdecl PathFree(void* p){dispose(p,0);}
void* __cdecl operator new(size_t n){return allocate(n,1);}
void* __cdecl operator new[](size_t n){return allocate(n,2);}
void __cdecl operator delete(void* p){dispose(p,1);}
void __cdecl operator delete[](void* p){dispose(p,2);}
extern "C" void* __cdecl MultimapMalloc(size_t size){require(size==24&&allocated<64);unsigned char* p=(unsigned char*)malloc(size+16);require(p!=0);memset(p,0x6b,size+16);memset(p+8,0xcc,size);payloads[allocated]=pendingPayload;blocks[allocated++]=(Node*)(p+8);return p+8;}
static void guard(Node* p){unsigned char* q=(unsigned char*)p;for(int i=0;i<8;++i)require(q[i-8]==0x6b&&q[i+24]==0x6b);}
char g_FableEmptyNarrowString_0129AAF4[1]={0};


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
// Cold siblings share these translation units; fail closed if reached.
CCharString::CCharString(){exit(90);}
const CCharString& CCharString::operator=(const char*){exit(90);return *this;}
CCharString& CCharString::operator=(const CCharString&){exit(90);return *this;}
CCharString CCharString::GetSubString(unsigned long,unsigned long)const{exit(90);return *this;}
long CCharString::FindCharPos(char)const{exit(90);return 0;}
CWideString::CWideString(const wchar_t*){exit(90);}
CWideString __fastcall CAFile::ExpandPath(const CWideString& a,const CWideString&){exit(90);return a;}
CWideString __fastcall CAFile::GetCurrentPath(){exit(90);return CWideString();}
CWideString __fastcall operator+(const CWideString& a,const CWideString&){exit(90);return a;}
bool __fastcall CAFile::PathExists(const CWideString&){exit(90);return false;}


fable_i32 g_CWideStringInstanceCount_013BCA20=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};

static int canonical(int k){if(k%8==0)return 0;for(int j=1;j<k;++j)if(j%8&&strcmp(inputText(j),inputText(k))==0)return j;return k;}
static int symbolId(const Node* p){CCharStringData* ref=*(CCharStringData* const*)&p->_M_value_field.first;if(!ref)return 0;for(int i=0;i<16;++i)if(ref==refs[i])return canonical(i);require(false);return 0;}
static bool symbolLess(int a,int b){if(a==0||b==0)return a==0&&b!=0;const signed char* x=(const signed char*)inputText(a);const signed char* y=(const signed char*)inputText(b);while(*x&&*x==*y){++x;++y;}return *x<*y;}
static int checkSymbol(Node* p,Node* parent){
 if(!p)return 1;int position=-1;for(int i=0;i<allocated;++i)if(blocks[i]==p)position=i;require(position>=0&&!visited[position]);visited[position]=true;guard(p);require(*(unsigned char*)p<=1&&p->_M_parent==parent&&p->_M_value_field.second==payloads[position]);
 int l=checkSymbol((Node*)p->_M_left,p);int key=symbolId(p);require(previous<0||symbolLess(previous,key));previous=key;++nodes;
 if(!p->_M_color)require((!p->_M_left||p->_M_left->_M_color)&&(!p->_M_right||p->_M_right->_M_color));int r=checkSymbol((Node*)p->_M_right,p);require(l==r);return l+(p->_M_color?1:0);
}
static unsigned crc(const char* p,unsigned n){unsigned value=0;for(unsigned i=0;i<n;++i){value^=(unsigned char)p[i];for(int bit=0;bit<8;++bit)value=(value>>1)^((value&1)?0xedb88320UL:0);}return value;}
static void checkTree(Tree* tree,_STL::_Rb_tree_node_base& header,int expected,bool symbol){
 require(tree->_M_node_count==expected);Node* root=(Node*)header._M_parent;if(!expected){require(!root&&header._M_left==&header&&header._M_right==&header);return;}
 require(root&&root->_M_color&&root->_M_parent==&header);previous=-1;nodes=0;memset(visited,0,sizeof(visited));if(symbol)checkSymbol(root,(Node*)&header);else check(root,(Node*)&header);require(nodes==expected);
 Node* left=root;while(left->_M_left)left=(Node*)left->_M_left;Node* right=root;while(right->_M_right)right=(Node*)right->_M_right;require(header._M_left==left&&header._M_right==right);
}
int main(){int cases=0;int zig[]={0,7,1,6,2,5,3,4},mix[]={3,1,6,0,7,2,5,4},counts[]={0,1,4,9,17,24};
 for(int c=0;c<6;++c)for(int pattern=0;pattern<4;++pattern)for(int rotation=0;rotation<8;++rotation)for(int alias=0;alias<2;++alias)for(int mask=0;mask<8;++mask){int count=counts[c];
  union{double align;char data[sizeof(CBankFile)];} mem;memset(&mem,0,sizeof(mem));CBankFile* bank=(CBankFile*)&mem;bank->OpenFlags=(mask&1?64:0)|(mask&2?128:0)|(mask&4?256:0);Tree* tree=&bank->FilenameIndices._M_t;Tree* symbols=&bank->SymbolIndices._M_t;
  _STL::_Rb_tree_node_base header,symbolHeader;memset(&header,0,sizeof(header));memset(&symbolHeader,0,sizeof(symbolHeader));header._M_left=header._M_right=&header;symbolHeader._M_left=symbolHeader._M_right=&symbolHeader;tree->_M_header._M_data=(Node*)&header;symbols->_M_header._M_data=(Node*)&symbolHeader;
  require(g_CCharStringInstanceCount_013BD800==0);allocationCount=0;
  for(int k=0;k<16;++k){const char* text=inputText(k)?inputText(k):"";size_t length=strlen(text);refs[k]=new CCharStringData;refs[k]->Data.PData=new char[length+1];memcpy(refs[k]->Data.PData,text,length+1);refs[k]->Data.StringLength=length;*(unsigned*)((char*)refs[k]+8)=0x80000000UL|(length+1);refs[k]->Data.UseFastExtend=true;refs[k]->owners=1;anchors[k]=refs[k];++g_CCharStringInstanceCount_013BD800;}
  allocated=released=conversions=lowerings=0;int expected=0,symbolCount=0,crcCount=0;int owners[16];Node* symbolNodes[16]={0};for(int k=0;k<16;++k)owners[k]=1;_STL::pair<unsigned long,unsigned long> crcExpected[32];unsigned* vector=(unsigned*)((char*)bank+0xd0);
  for(int i=0;i<count;++i){int key=((pattern==0?i%8:pattern==1?7-i%8:pattern==2?zig[i%8]:mix[i%8])+rotation)%8;int storageKey=key+(alias&&(i&1)?8:0);void* value=key?refs[storageKey]:0;int symbolKey=canonical(storageKey);int before=allocated;
   bool symbolNew=(mask&1)&&!symbolNodes[symbolKey];pendingPayload=0xffffffffUL-i;CallInit(bank,0,pendingPayload,*(CCharString*)&value,*(CCharString*)&value);
   if(symbolNew){require(allocated>before);symbolNodes[symbolKey]=blocks[before];++symbolCount;if(key)++owners[storageKey];require(symbolId(blocks[before])==symbolKey);}
   if(mask&1)for(int j=0;j<allocated;++j)if(blocks[j]==symbolNodes[symbolKey])payloads[j]=pendingPayload;
   bool inserted=(mask&2)&&key!=0&&key!=3;if(inserted){require(allocated>before+(symbolNew?1:0));++expected;require(id(blocks[allocated-1])==key);}
   require(allocated==symbolCount+expected&&g_CCharStringInstanceCount_013BD800==16+symbolCount+expected&&g_CWideStringInstanceCount_013BCA20==0);
   for(int j=0;j<16;++j)require(refs[j]->owners==owners[j]&&strcmp(refs[j]->Data.PData,inputText(j)?inputText(j):"")==0);
   checkTree(tree,header,expected,false);checkTree(symbols,symbolHeader,symbolCount,true);
   if(mask&4){crcExpected[crcCount].first=crc(value?inputText(storageKey):"",value?(unsigned)strlen(inputText(storageKey)):0);crcExpected[crcCount++].second=pendingPayload;}
   require(vector[1]-vector[0]==crcCount*8&&vector[1]<=vector[2]&&bank->SymbolCRCIndices.Dirty==(crcCount!=0));if(crcCount)require(memcmp((void*)vector[0],crcExpected,crcCount*8)==0);
  }
  release((Node*)header._M_parent);release((Node*)symbolHeader._M_parent);PathFree((void*)vector[0]);require(released==allocated&&g_CCharStringInstanceCount_013BD800==16);
  for(int k=0;k<16;++k)require(refs[k]->owners==1);for(int k=0;k<16;++k){((CCharString*)&anchors[k])->~CCharString();require(anchors[k]==0);}require(g_CCharStringInstanceCount_013BD800==0&&g_CWideStringInstanceCount_013BCA20==0);for(unsigned k=0;k<allocationCount;++k)require(allocations[k].freed);++cases;
 }
 printf("NATIVE_INIT_COMPLETE_PASS cases=%d\n",cases);return 0;
}
