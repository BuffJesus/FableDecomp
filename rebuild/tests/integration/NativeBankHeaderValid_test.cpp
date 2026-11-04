#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "fable_bank_entry_update.h"
#include "fable_bank_file_data_type_count.h"
#include "fable_filesystem.h"
// Native entry parsing, allocation, all indices, packed/runtime selection,
// metadata queries, strings and buffered input. Independent per-bit decoding,
// bias/width/footprint expectations and reserved-index0 checks.
// Tree headers are fixtures; native clear releases nodes and packed arrays;
// vectors are manually released. Notifications, CAFile and base lifetime remain
// observed. Cases: NativeBankEntryIndexed_model.py; 64 flag combinations.
static void require(bool ok,int line){if(!ok){fprintf(stderr,"BANK_FULL_HEADER_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
struct Block{unsigned char* p;size_t n;int kind;bool freed;};static Block blocks[4096];static unsigned used;
static void* allocate(size_t n,int kind){CHECK(used<4096&&n<0x10000);unsigned char* p=(unsigned char*)malloc(n+32);CHECK(p!=0);memset(p,0x7d,n+32);Block& b=blocks[used++];b.p=p+16;b.n=n;b.kind=kind;b.freed=false;return b.p;}
static void dispose(void* p,int kind){if(!p)return;Block* b=0;for(unsigned i=used;i-->0;)if(blocks[i].p==p&&!blocks[i].freed){b=&blocks[i];break;}CHECK(b&&b->kind==kind);for(int i=0;i<16;++i)CHECK(b->p[i-16]==0x7d&&b->p[b->n+i]==0x7d);b->freed=true;free(b->p-16);}
extern "C" void* __cdecl FixtureVectorMalloc(size_t n){return allocate(n,0);}
extern "C" void __cdecl FixtureVectorFree(void* p){dispose(p,0);}
void* __cdecl operator new(size_t n){return allocate(n,1);}
void* __cdecl operator new[](size_t n){return allocate(n,2);}
void __cdecl operator delete(void* p){dispose(p,1);}
void __cdecl operator delete[](void* p){dispose(p,2);}
static unsigned* W(void* p){return static_cast<unsigned*>(p);}
// Case file reader.
static unsigned* g_words;static unsigned g_at,g_wordCount;
static unsigned Next(){CHECK(g_at<g_wordCount);return g_words[g_at++];}
static const unsigned char* Bytes(unsigned& n){n=Next();CHECK(n<=0x10000&&(n+3)/4<=g_wordCount-g_at);const unsigned char* p=(const unsigned char*)(g_words+g_at);g_at+=(n+3)/4;return p;}
// The bank: raw retail layout with a fake vtable (slots 10/12/15 observed, others trap).
static unsigned char bank[0x110],before[0x110];static void* bankVtable[20];
static unsigned g_flags,g_readOnly,g_size,g_entryIndex,g_entries,g_events[8],g_typeCount;static const unsigned* g_types;
enum{AllocateEvent,BankSizeEvent,InitEvent,AddedEvent,PackEvent,PostEvent};
static unsigned g_order[4096],g_orderCount;static unsigned char runtimePadding[128][2];
static void Event(unsigned e){CHECK(g_orderCount<4096);g_order[g_orderCount++]=e;++g_events[e];}
extern "C" void __cdecl BankTrap(){CHECK(false);}
// Native buffered stream over a controlled backing CAFile.
static unsigned char stream[0x40];static const unsigned char* g_data;static unsigned g_length,g_chunk,g_overflows,g_moves;
struct BackingFile {void** table;unsigned access;unsigned position,length;void* handle;bool open;unsigned char padding[3];unsigned pathname;};static BackingFile file;static void* fileTable[16];
extern "C" void ColdBoundary(){CHECK(false);}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
static void __fastcall FileSeek(BackingFile* self,void*,long position){CHECK(self==&file&&position>=0&&(unsigned)position<=g_length);self->position=position;++g_moves;}
static long __fastcall FilePosition(BackingFile* self,void*){CHECK(self==&file);return self->position;}
static long __fastcall FileLength(BackingFile* self,void*){CHECK(self==&file);return g_length;}
static bool __fastcall FileOpen(BackingFile* self,void*){CHECK(self==&file);return true;}
static void __fastcall FileRead(BackingFile* self,void*,void* out,long n,void* process){CHECK(self==&file&&!process&&n>=0&&self->position+(unsigned)n<=g_length);CHECK((unsigned)n>=g_chunk||self->position+(unsigned)n==g_length);memcpy(out,g_data+self->position,n);self->position+=n;++g_overflows;}
extern "C" __declspec(dllimport) void* __stdcall CreateFileA(const char*,unsigned,unsigned,void*,unsigned,unsigned,void*);
extern "C" __declspec(dllimport) int __stdcall WriteFile(void*,const void*,unsigned,unsigned*,void*);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
extern "C" __declspec(dllimport) unsigned __stdcall GetLastError();
extern "C" __declspec(dllimport) unsigned __stdcall SetFilePointer(void*,long,long*,unsigned);
extern "C" void __fastcall DiskRead(BackingFile*,void*,void*,long,void*);
extern "C" void __fastcall DiskSeek(BackingFile*,void*,long);
extern "C" long __fastcall DiskPosition(BackingFile*,void*);
extern "C" void __fastcall DiskClose(BackingFile*,void*);
#pragma comment(linker,"/alternatename:@DiskRead@20=?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z")
#pragma comment(linker,"/alternatename:@DiskSeek@12=?SetPosition@CDiskFileWin32@@UAEXJ@Z")
#pragma comment(linker,"/alternatename:@DiskPosition@8=?GetPosition@CDiskFileWin32@@UBEJXZ")
#pragma comment(linker,"/alternatename:@DiskClose@8=?Close@CDiskFileWin32@@UAEXXZ")
static void StreamSet(void* self,void*,unsigned long pos){CHECK(self==stream);reinterpret_cast<CFileDataInputStream*>(stream)->SetPosition(pos);}
#include "NativeFileInputStreamColdAliases.h"
// Current expected entry.
struct Expect{unsigned index,type,size,offset,crc,timestamp,symbolLen,names,infoLen,end;const unsigned char* symbol;const unsigned char* name0;unsigned name0Len;const unsigned char* info;const unsigned char* nameBytes[8];unsigned nameLens[8];};
static Expect g_expect[64];
static bool Same(const CCharString& s,const unsigned char* bytes,unsigned n){const char* text=s;return strlen(text)==n&&!memcmp(text,bytes,n);}
static void CheckNativeArray(unsigned offset,unsigned width,unsigned flag){unsigned* v=W(bank+offset);if(flag&&!(g_flags&flag)){CHECK(!v[0]&&!v[1]&&!v[2]);return;}CHECK(v[0]&&v[1]-v[0]==width*g_size&&v[2]==v[1]);Block* found=0;for(unsigned i=0;i<used;++i)if((unsigned)blocks[i].p==v[0]&&!blocks[i].freed)found=&blocks[i];CHECK(found&&found->kind==0&&found->n==width*g_size);for(int n=0;n<16;++n)CHECK(found->p[n-16]==0x7d&&found->p[found->n+n]==0x7d);}
extern "C" void __fastcall BankSize(void* self,void*,unsigned long size,const CArray<CBankFileDataTypeCount>& counts){CHECK(self==bank&&size==g_size&&g_orderCount==0);Event(AllocateEvent);Event(BankSizeEvent);CHECK(counts.size()==g_typeCount);for(unsigned i=0;i<g_typeCount;++i)CHECK(counts[i].DataType==g_types[2*i]&&counts[i].Count==g_types[2*i+1]);CheckNativeArray(8,4,8);CheckNativeArray(0x14,4,16);CheckNativeArray(0x20,12,0);CheckNativeArray(0x2c,4,32);CHECK(g_size<128);for(unsigned j=0;j<g_size;++j){unsigned char* p=(unsigned char*)W(bank+0x20)[0]+12*j;CHECK(W(p)[0]==0&&W(p)[1]==0&&p[8]==0&&p[9]==0);if(g_flags&8)CHECK(((unsigned*)W(bank+8)[0])[j]==0);if(g_flags&16)CHECK(((unsigned*)W(bank+0x14)[0])[j]==0);if(g_flags&32)CHECK(((unsigned*)W(bank+0x2c)[0])[j]==0);runtimePadding[j][0]=p[10];runtimePadding[j][1]=p[11];}}
extern "C" void __fastcall Added(void* self,void*,unsigned long type,unsigned long index,const CArray<char>& info){CHECK(self==bank&&g_entryIndex<g_entries);Expect& x=g_expect[g_entryIndex];Event(InitEvent);Event(AddedEvent);
 CHECK(type==x.type&&index==x.index&&info.size()==x.infoLen&&(!x.infoLen||!memcmp(&info[0],x.info,x.infoLen)));
 // A handler may move the stream; ReadEntryList must put it back.
 ++g_entryIndex;}
extern "C" void __fastcall Post(void* self,void*){CHECK(self==bank&&bank[0x94]==before[0x94]);if(g_readOnly)Event(PackEvent);Event(PostEvent);}
extern "C" bool __fastcall HeaderRead(void*,void*,bool);
#pragma comment(linker,"/alternatename:@HeaderRead@12=?ReadFileInformation@CBankFile@@AAE_N_N@Z")
static unsigned __fastcall Format(void* self,void*){CHECK(self==bank&&file.position==g_length&&SetFilePointer(file.handle,0,0,1)==g_length);return 19;}
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@0@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@PBD@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GFIntToCharString@@YI?AVCCharString@@J@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?ReadRecoveryFile@CBankFile@@AAE_NK@Z=_ColdBoundary")
// Cold siblings from the native pathname unit.
const CCharString& CCharString::operator=(const char*){ColdBoundary();return *this;}
CCharString CCharString::GetSubString(unsigned long,unsigned long)const{ColdBoundary();return *this;}
long CCharString::FindCharPos(char)const{ColdBoundary();return 0;}
CWideString::CWideString(const wchar_t*){ColdBoundary();}
CWideString __fastcall CAFile::ExpandPath(const CWideString& a,const CWideString&){ColdBoundary();return a;}
CWideString __fastcall CAFile::GetCurrentPath(){ColdBoundary();return CWideString();}
CWideString __fastcall operator+(const CWideString& a,const CWideString&){ColdBoundary();return a;}
bool __fastcall CAFile::PathExists(const CWideString&){ColdBoundary();return false;}
fable_i32 g_CWideStringInstanceCount_013BCA20=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
typedef _STL::pair<const CCharString,unsigned long> IndexedPair;
typedef _STL::_Rb_tree_node<IndexedPair> IndexedNode;
typedef _STL::map<CCharString,unsigned long>::_Rep_type IndexedTree;
struct NodeExpectation {const unsigned char* key;unsigned length,payload;};
static NodeExpectation expectedNodes[2][64];static unsigned expectedCounts[2];
static _STL::_Rb_tree_node_base treeHeaders[2];static IndexedNode* visitedNodes[128];static unsigned visitedCount;
static void NodeGuard(IndexedNode* node){Block* found=0;for(unsigned i=0;i<used;++i)if((void*)blocks[i].p==node&&!blocks[i].freed)found=&blocks[i];CHECK(found&&found->kind==0&&found->n==sizeof(IndexedNode));for(int j=0;j<16;++j)CHECK(found->p[j-16]==0x7d&&found->p[found->n+j]==0x7d);}
static int CheckNode(IndexedNode* node,_STL::_Rb_tree_node_base* parent,unsigned which,unsigned& position){if(!node)return 1;CHECK(visitedCount<128);for(unsigned i=0;i<visitedCount;++i)CHECK(visitedNodes[i]!=node);visitedNodes[visitedCount++]=node;NodeGuard(node);CHECK(node->_M_parent==parent&&node->_M_color<=1);int left=CheckNode((IndexedNode*)node->_M_left,node,which,position);CHECK(position<expectedCounts[which]);NodeExpectation& expected=expectedNodes[which][position++];CHECK(Same(node->_M_value_field.first,expected.key,expected.length)&&node->_M_value_field.second==expected.payload);int right=CheckNode((IndexedNode*)node->_M_right,node,which,position);CHECK(left==right);if(!node->_M_color){if(node->_M_left)CHECK(node->_M_left->_M_color);if(node->_M_right)CHECK(node->_M_right->_M_color);}return left+(node->_M_color?1:0);}
static void CheckTree(unsigned which){IndexedTree* tree=(IndexedTree*)(bank+(which?0xc4:0xac));CHECK(tree->_M_header._M_data==&treeHeaders[which]&&tree->_M_node_count==expectedCounts[which]);IndexedNode* root=(IndexedNode*)treeHeaders[which]._M_parent;if(root){NodeGuard(root);CHECK(root->_M_color);unsigned position=0;visitedCount=0;CheckNode(root,&treeHeaders[which],which,position);CHECK(position==expectedCounts[which]);IndexedNode* left=root;while(left->_M_left)left=(IndexedNode*)left->_M_left;IndexedNode* right=root;while(right->_M_right)right=(IndexedNode*)right->_M_right;CHECK(treeHeaders[which]._M_left==left&&treeHeaders[which]._M_right==right);}else CHECK(!expectedCounts[which]&&treeHeaders[which]._M_left==&treeHeaders[which]&&treeHeaders[which]._M_right==&treeHeaders[which]);}
extern void __fastcall ClearPackedBankArray(CPackedUIntArray*);
static unsigned ExpectedMetadata(unsigned index,unsigned field){int last=-1;for(unsigned e=0;e<g_entries;++e)if(g_expect[e].index==index)last=e;if(last<0)return 0;Expect& x=g_expect[last];return field==0?x.offset:field==1?x.size:field==2?(unsigned char)x.type:1;}
static unsigned DecodePacked(const CPackedUIntArray& p,unsigned index){unsigned value=0;for(unsigned bit=0;bit<p.Bits;++bit){unsigned at=index*p.Bits+bit;if((p.PackedInts[at/32]>>(at%32))&1)value|=1UL<<bit;}return value+p.Bias;}
static bool CheckRepresentation(){unsigned bias[4]={0},bits[4]={0},packedBytes=0;for(unsigned field=0;field<4;++field){if(g_size>1){unsigned lo=0xffffffff,hi=0;for(unsigned i=1;i<g_size;++i){unsigned value=ExpectedMetadata(i,field);if(value<lo)lo=value;if(value>hi)hi=value;}bias[field]=lo;unsigned range=hi-lo;while(range){++bits[field];range>>=1;}}packedBytes+=16+(((g_size-1)*bits[field]+31)/32)*4;}bool keep=g_readOnly&&g_size>1&&packedBytes<(g_size+1)*12;CHECK((W(bank+0x20)[0]==0)==keep);if(keep)CHECK(W(bank+0x20)[1]==0&&W(bank+0x20)[2]==0);else CheckNativeArray(0x20,12,0);for(unsigned field=0;field<4;++field){CPackedUIntArray& p=*(CPackedUIntArray*)(bank+0x38+16*field);CHECK(!p.Padding09[0]&&!p.Padding09[1]&&!p.Padding09[2]);if(keep){CHECK(p.Size==g_size-1&&p.Bias==bias[field]&&p.Bits==bits[field]);unsigned bytes=((p.Size*p.Bits+31)/32)*4;CHECK(p.Sizeof()==16+bytes);if(p.Bits){Block* found=0;for(unsigned n=0;n<used;++n)if((void*)blocks[n].p==p.PackedInts&&!blocks[n].freed)found=&blocks[n];CHECK(found&&found->kind==2&&found->n==bytes);unsigned partial=(p.Size*p.Bits)%32;if(partial)CHECK((p.PackedInts[bytes/4-1]>>partial)==0);}else CHECK(!p.PackedInts);for(unsigned i=1;i<g_size;++i)CHECK(p[i-1]==ExpectedMetadata(i,field)&&DecodePacked(p,i-1)==ExpectedMetadata(i,field));}else CHECK(!p.PackedInts&&!p.Size&&!p.Bits&&!p.Bias);}return keep;}
char g_FableEmptyNarrowString_0129AAF4[1]={0};
int main(int argc,char** argv){
 CHECK(argc==3);FILE* f=fopen(argv[1],"rb");CHECK(f!=0);fseek(f,0,SEEK_END);long bytes=ftell(f);CHECK(bytes>0&&bytes%4==0);g_wordCount=(unsigned)bytes/4;fseek(f,0,SEEK_SET);g_words=(unsigned*)malloc(bytes);CHECK(g_words!=0&&fread(g_words,1,bytes,f)==(size_t)bytes);fclose(f);
 for(int i=0;i<20;++i)bankVtable[i]=(void*)BankTrap;bankVtable[8]=(void*)Format;bankVtable[10]=(void*)BankSize;bankVtable[12]=(void*)Added;bankVtable[15]=(void*)Post;
 for(int i=0;i<16;++i)fileTable[i]=(void*)ColdBoundary;fileTable[3]=(void*)DiskRead;fileTable[5]=(void*)DiskSeek;fileTable[7]=(void*)DiskPosition;fileTable[9]=(void*)FileLength;fileTable[10]=(void*)FileOpen;
 unsigned count=Next(),cases=0,entries=0,updates=0,overflowCases=0,packs=0,sorted=0,packedCases=0,runtimeCases=0,noOpCases=0;
 for(unsigned c=0;c<count;++c){
  g_flags=Next();g_readOnly=Next();g_chunk=Next();unsigned maxIndex=Next();g_size=maxIndex+1;g_entries=Next();CHECK(g_entries<=64);g_data=Bytes(g_length);unsigned char* whole=(unsigned char*)calloc(g_length+64,1);CHECK(whole);memcpy(whole+64,g_data,g_length);g_data=whole;g_length+=64;W(whole)[0]=0x42424242;W(whole)[1]=7;W(whole)[2]=19;W(whole)[3]=1;W(whole)[4]=4;W(whole)[5]=maxIndex;W(whole)[6]=g_entries;W(whole)[7]=64;
  g_typeCount=Next();g_types=g_words+g_at;g_at+=2*g_typeCount;
  unsigned crcCount=Next(),crcDirty=Next(),crcExtra=Next();const unsigned* crcPairs=g_words+g_at;g_at+=2*crcCount;
  for(unsigned e=0;e<g_entries;++e){Expect& x=g_expect[e];x.index=Next();x.type=Next();x.size=Next();x.offset=Next();x.crc=Next();x.timestamp=Next();x.symbol=Bytes(x.symbolLen);x.names=Next();CHECK(x.names<=8);
   for(unsigned n=0;n<x.names;++n)x.nameBytes[n]=Bytes(x.nameLens[n]);x.name0=x.names?x.nameBytes[0]:0;x.name0Len=x.names?x.nameLens[0]:0;x.info=Bytes(x.infoLen);x.end=Next();}
  unsigned wantCount=Next();const unsigned* want=g_words+g_at;g_at+=2*wantCount;for(unsigned which=0;which<2;++which){expectedCounts[which]=Next();CHECK(expectedCounts[which]<=64);for(unsigned k=0;k<expectedCounts[which];++k){expectedNodes[which][k].key=Bytes(expectedNodes[which][k].length);expectedNodes[which][k].payload=Next();}}
  used=0;memset(bank,0x6b,sizeof(bank));memset(bank+8,0,0x30);for(unsigned which=0;which<2;++which){unsigned* tree=W(bank+(which?0xc4:0xac));tree[0]=(unsigned)&treeHeaders[which];tree[1]=0;treeHeaders[which]._M_color=0;treeHeaders[which]._M_parent=0;treeHeaders[which]._M_left=treeHeaders[which]._M_right=&treeHeaders[which];}W(bank)[0]=(unsigned)bankVtable;memset(bank+0x38,0,0x40);W(bank+0x78)[0]=g_flags;bank[0x8c]=0;bank[0x8d]=(unsigned char)g_readOnly;bank[0x94]=0;
  unsigned* crc=crcCount+crcExtra?(unsigned*)FixtureVectorMalloc(8*(crcCount+crcExtra)):0;if(crcCount)memcpy(crc,crcPairs,8*crcCount);W(bank+0xd0)[0]=(unsigned)crc;W(bank+0xd0)[1]=(unsigned)(crc+2*crcCount);W(bank+0xd0)[2]=(unsigned)(crc+2*(crcCount+crcExtra));bank[0xdc]=0;bank[0xdd]=(unsigned char)crcDirty;
  file.table=fileTable;file.access=1;file.position=7;file.length=g_length;file.open=true;
  void* writer=CreateFileA(argv[2],0x40000000,0,0,2,0x80,0);CHECK(writer!=(void*)-1);unsigned written=0;CHECK(WriteFile(writer,whole,g_length,&written,0)&&written==g_length&&CloseHandle(writer));file.handle=CreateFileA(argv[2],0x80000000,1,0,3,0x80,0);CHECK(file.handle!=(void*)-1&&SetFilePointer(file.handle,7,0,0)==7);
  W(bank+0x7c)[0]=(unsigned)&file;W(bank+0x80)[0]=0;new(bank+0x84) CWideString();W(bank+0x98)[0]=4;W(bank+0x9c)[0]=7;W(bank+0xec)[0]=0;
  memcpy(before,bank,sizeof(bank));g_orderCount=0;g_entryIndex=0;g_overflows=0;g_moves=0;memset(g_events,0,sizeof(g_events));
  bool result=HeaderRead(bank,0,false);CHECK(g_CWideStringInstanceCount_013BCA20==1);reinterpret_cast<CWideString*>(bank+0x84)->~CWideString();
  CHECK(result&&bank[0x94]==1&&file.position==g_length&&W(bank+0xa0)[0]==32&&W(bank+0xa8)[0]==g_length&&g_entryIndex==g_entries);
  // Event order: allocate, bank size, (init, added) per entry, pack if read-only, post.
  CHECK(g_orderCount==3+2*g_entries+(g_readOnly?1:0)&&g_order[0]==AllocateEvent&&g_order[1]==BankSizeEvent&&g_order[g_orderCount-1]==PostEvent);
  for(unsigned e=0;e<g_entries;++e)CHECK(g_order[2+2*e]==InitEvent&&g_order[3+2*e]==AddedEvent);if(g_readOnly)CHECK(g_order[g_orderCount-2]==PackEvent);
  CHECK(g_size==maxIndex+1);
  // Untouched fields outside the arrays, flags and the CRC map.
  for(unsigned i=0;i<sizeof(bank);++i)if(!(i>=4&&i<0x38)&&i!=0x94&&!(i>=0x38&&i<0x78)&&!(i>=0xa0&&i<0xa4)&&!(i>=0xa8&&i<0xac)&&!(i>=0xac&&i<0xb4)&&!(i>=0xc4&&i<0xcc)&&!(i>=0xd0&&i<0xde))CHECK(bank[i]==before[i]);
  bool retainedPacked=CheckRepresentation();packedCases+=retainedPacked;runtimeCases+=g_readOnly&&!retainedPacked&&g_size>1;noOpCases+=g_readOnly&&g_size<=1;
  CBankFile* typed=(CBankFile*)bank;unsigned expectedCount=0,expectedTotal=0;CHECK(!typed->EntryExists(0)&&!typed->EntryExists(g_size)&&!typed->EntryExists(0xffffffff));for(unsigned i=1;i<g_size;++i){CHECK(typed->GetEntryDataOffset(i)==ExpectedMetadata(i,0)&&typed->GetEntryDataSize(i)==ExpectedMetadata(i,1)&&typed->GetEntryDataType(i)==ExpectedMetadata(i,2)&&typed->EntryExists(i)==(ExpectedMetadata(i,3)!=0));if(ExpectedMetadata(i,3)){++expectedCount;expectedTotal+=ExpectedMetadata(i,1);}}CHECK(typed->CountEntries()==expectedCount&&typed->GetTotalDataSize()==expectedTotal);
  // Each array slot holds the last record written to it.
  CCharString* symbols=(CCharString*)W(bank+8)[0];unsigned* sums=(unsigned*)W(bank+0x14)[0];unsigned char* runtime=(unsigned char*)W(bank+0x20)[0];CBankFileEntryUpdateData** upd=(CBankFileEntryUpdateData**)W(bank+0x2c)[0];
  for(unsigned i=0;i<g_size;++i){int last=-1;for(unsigned e=0;e<g_entries;++e)if(g_expect[e].index==i)last=(int)e;
   if(last<0){CHECK((!runtime||(!W(runtime+12*i)[0]&&!W(runtime+12*i)[1]&&!runtime[12*i+8]&&!runtime[12*i+9]))&&(!(g_flags&8)||Same(symbols[i],(const unsigned char*)"",0))&&(!(g_flags&16)||!sums[i])&&(!(g_flags&32)||!upd[i]));continue;}
   Expect& x=g_expect[last];
   CHECK(g_flags&8?Same(symbols[i],x.symbol,x.symbolLen):symbols==0);CHECK(g_flags&16?sums[i]==x.timestamp:sums==0);
   if(g_flags&32){CBankFileEntryUpdateData* u=upd[i];CHECK(u&&u->StateBlockCRC==x.crc&&u->InfoSize==x.infoLen&&(x.infoLen?u->InfoBuffer&&!memcmp(u->InfoBuffer,x.info,x.infoLen):!u->InfoBuffer)&&!u->RequiresUpdate&&u->Exists);
    CHECK(u->Filenames.Size==x.names&&(!x.names||u->Filenames.Data!=0));for(unsigned n=0;n<x.names;++n)CHECK(Same(u->Filenames.Data[n],x.nameBytes[n],x.nameLens[n]));
    delete[] u->InfoBuffer;u->Filenames.Clear();delete u;++updates;}else CHECK(!upd);
  }
  if(runtime)for(unsigned j=0;j<g_size;++j)CHECK(runtime[12*j+10]==runtimePadding[j][0]&&runtime[12*j+11]==runtimePadding[j][1]);
  // CRC map: sorted when dirty, then trimmed to its size.
  unsigned* begin=(unsigned*)W(bank+0xd0)[0];unsigned* end=(unsigned*)W(bank+0xd0)[1];unsigned* cap=(unsigned*)W(bank+0xd0)[2];
  CHECK((unsigned)(end-begin)==2*wantCount&&cap==end&&bank[0xdd]==0&&bank[0xdc]==0);for(unsigned i=0;i<2*wantCount;++i)CHECK(begin[i]==want[i]);if((crcDirty||((g_flags&256)&&g_entries))&&wantCount>1)++sorted;
  CheckTree(0);CheckTree(1);CHECK(g_CWideStringInstanceCount_013BCA20==0);((IndexedTree*)(bank+0xac))->clear();((IndexedTree*)(bank+0xc4))->clear();CHECK(W(bank+0xac)[1]==0&&W(bank+0xc4)[1]==0);
  for(unsigned field=0;field<4;++field)ClearPackedBankArray((CPackedUIntArray*)(bank+0x38+16*field));
  FixtureVectorFree(begin);if(g_flags&8)for(unsigned i=0;i<g_size;++i)symbols[i].~CCharString();FixtureVectorFree(symbols);FixtureVectorFree(sums);FixtureVectorFree(runtime);FixtureVectorFree(upd);
  CHECK(SetFilePointer(file.handle,0,0,1)==g_length);void* oldHandle=file.handle;DiskClose(&file,0);CHECK(!file.open&&!CloseHandle(oldHandle)&&GetLastError()==6);DiskClose(&file,0);free(whole);
  for(unsigned i=0;i<used;++i)CHECK(blocks[i].freed);CHECK(g_CCharStringInstanceCount_013BD800==0);
  entries+=g_entries;overflowCases+=g_overflows>0;packs+=g_readOnly;++cases;
 }
 CHECK(g_at==g_wordCount);free(g_words);
 printf("BANK_FULL_HEADER_PASS cases=%u entries=%u updates=%u overflow=%u packs=%u sorted=%u packed=%u runtime=%u noop=%u\n",cases,entries,updates,overflowCases,packs,sorted,packedCases,runtimeCases,noOpCases);return 0;
}
