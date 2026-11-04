#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
#include "fable_data_input_stream.h"
#include "fable_bank_entry_update.h"
#include "fable_bank_file_data_type_count.h"
// Real: ReadEntryList, ReadPresizedString, CCharString, CSmallVector, the vector(n)
// constructors, CVectorMap Sort/DeallocateUnusedMemory. Observed boundaries (accepted by
// their own gates): AllocateBuffers, InitEntry, PackRuntimeData, the chunk-overflow reader
// and the stream/bank virtual slots. Cases come from NativeBankEntryList_model.py.
static void require(bool ok,int line){if(!ok){fprintf(stderr,"BANK_ENTRY_LIST_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
struct Block{unsigned char* p;size_t n;int kind;bool freed;};static Block blocks[4096];static unsigned used;
static void* allocate(size_t n,int kind){CHECK(used<4096&&n<0x10000);unsigned char* p=(unsigned char*)malloc(n+32);CHECK(p!=0);memset(p,0x7d,n+32);Block& b=blocks[used++];b.p=p+16;b.n=n;b.kind=kind;b.freed=false;return b.p;}
static void dispose(void* p,int kind){if(!p)return;Block* b=0;for(unsigned i=used;i-->0;)if(blocks[i].p==p&&!blocks[i].freed){b=&blocks[i];break;}CHECK(b&&b->kind==kind);for(int i=0;i<16;++i)CHECK(b->p[i-16]==0x7d&&b->p[b->n+i]==0x7d);b->freed=true;free(b->p-16);}
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
static unsigned g_order[4096],g_orderCount;
static void Event(unsigned e){CHECK(g_orderCount<4096);g_order[g_orderCount++]=e;++g_events[e];}
extern "C" void __cdecl BankTrap(){CHECK(false);}
// Stream: retail CDataInputStream fields over a case buffer, windows of `chunk` bytes.
static unsigned char stream[0x40];static void* streamVtable[12];static const unsigned char* g_data;static unsigned g_length,g_chunk,g_overflows,g_moves;
static void Window(unsigned pos){unsigned left=g_length-pos;W(stream)[1]=pos;W(stream+0xc)[0]=(unsigned)(g_data+pos);W(stream+0x14)[0]=left<g_chunk?left:g_chunk;}
extern "C" void __fastcall StreamOverflow(void* self,void*,void* data,long size){CHECK(self==stream&&size>0);unsigned pos=W(stream)[1];CHECK(pos+size<=g_length);memcpy(data,g_data+pos,size);Window(pos+size);++g_overflows;}
#pragma comment(linker,"/alternatename:?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z=@StreamOverflow@16")
extern "C" void __fastcall StreamSet(void* self,void*,unsigned long pos){CHECK(self==stream&&pos<=g_length);Window(pos);++g_moves;}
extern "C" unsigned long __fastcall StreamGet(void* self,void*){CHECK(self==stream);return W(stream)[1];}
// Current expected entry.
struct Expect{unsigned index,type,size,offset,crc,timestamp,symbolLen,names,infoLen,end;const unsigned char* symbol;const unsigned char* name0;unsigned name0Len;const unsigned char* info;const unsigned char* nameBytes[8];unsigned nameLens[8];};
static Expect g_expect[64];
static bool Same(const CCharString& s,const unsigned char* bytes,unsigned n){const char* text=s;return strlen(text)==n&&!memcmp(text,bytes,n);}
extern "C" void __fastcall Allocate(void* self,void*,unsigned long size){CHECK(self==bank&&g_orderCount==0);Event(AllocateEvent);g_size=size;W(bank)[1]=size;
 CCharString* symbols=(CCharString*)malloc(4*size+4);for(unsigned i=0;i<size;++i)new(&symbols[i]) CCharString();W(bank+8)[0]=(unsigned)symbols;W(bank+8)[1]=W(bank+8)[2]=(unsigned)(symbols+size);
 unsigned* sums=(unsigned*)calloc(size+1,4);W(bank+0x14)[0]=(unsigned)sums;W(bank+0x14)[1]=W(bank+0x14)[2]=(unsigned)(sums+size);
 unsigned char* runtime=(unsigned char*)calloc(size+1,12);W(bank+0x20)[0]=(unsigned)runtime;W(bank+0x20)[1]=W(bank+0x20)[2]=(unsigned)(runtime+12*size);
 void** updates=(void**)calloc(size+1,4);W(bank+0x2c)[0]=(unsigned)updates;W(bank+0x2c)[1]=W(bank+0x2c)[2]=(unsigned)(updates+size);}
#pragma comment(linker,"/alternatename:?AllocateBuffers@CBankFile@@AAEXK@Z=@Allocate@12")
extern "C" void __fastcall BankSize(void* self,void*,unsigned long size,const CArray<CBankFileDataTypeCount>& counts){CHECK(self==bank&&size==g_size&&g_orderCount==1);Event(BankSizeEvent);CHECK(counts.size()==g_typeCount);for(unsigned i=0;i<g_typeCount;++i)CHECK(counts[i].DataType==g_types[2*i]&&counts[i].Count==g_types[2*i+1]);}
extern "C" void __fastcall Init(void* self,void*,unsigned long index,const CCharString& symbol,const CCharString& filename){CHECK(self==bank&&g_entryIndex<g_entries);Expect& x=g_expect[g_entryIndex];Event(InitEvent);
 CHECK(index==x.index&&Same(symbol,x.symbol,x.symbolLen)&&(x.names?Same(filename,x.name0,x.name0Len):Same(filename,(const unsigned char*)"",0))&&W(stream)[1]==x.end);
 // Retail fills the runtime record and the flagged arrays before indexing the entry.
 unsigned char* r=(unsigned char*)W(bank+0x20)[0]+12*index;CHECK(W(r)[0]==x.offset&&W(r)[1]==x.size&&r[8]==(unsigned char)x.type&&r[9]==1);}
#pragma comment(linker,"/alternatename:?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z=@Init@20")
extern "C" void __fastcall Added(void* self,void*,unsigned long type,unsigned long index,const CArray<char>& info){CHECK(self==bank&&g_entryIndex<g_entries);Expect& x=g_expect[g_entryIndex];Event(AddedEvent);
 CHECK(type==x.type&&index==x.index&&info.size()==x.infoLen&&(!x.infoLen||!memcmp(&info[0],x.info,x.infoLen))&&W(stream)[1]==x.end);
 // A handler may move the stream; ReadEntryList must put it back.
 StreamSet(stream,0,0);++g_entryIndex;}
extern "C" void __fastcall Pack(void* self,void*){CHECK(self==bank&&g_readOnly);Event(PackEvent);}
#pragma comment(linker,"/alternatename:?PackRuntimeData@CBankFile@@AAEXXZ=@Pack@8")
extern "C" void __fastcall Post(void* self,void*){CHECK(self==bank&&bank[0x94]==before[0x94]);Event(PostEvent);}
extern "C" bool __fastcall ReadList(void*,void*,CFileDataInputStream&,unsigned long,unsigned long);
#pragma comment(linker,"/alternatename:@ReadList@20=?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z")
char g_FableEmptyNarrowString_0129AAF4[1]={0};
int main(int argc,char** argv){
 CHECK(argc==2);FILE* f=fopen(argv[1],"rb");CHECK(f!=0);fseek(f,0,SEEK_END);long bytes=ftell(f);CHECK(bytes>0&&bytes%4==0);g_wordCount=(unsigned)bytes/4;fseek(f,0,SEEK_SET);g_words=(unsigned*)malloc(bytes);CHECK(g_words!=0&&fread(g_words,1,bytes,f)==(size_t)bytes);fclose(f);
 for(int i=0;i<20;++i)bankVtable[i]=(void*)BankTrap;bankVtable[10]=(void*)BankSize;bankVtable[12]=(void*)Added;bankVtable[15]=(void*)Post;
 for(int i=0;i<12;++i)streamVtable[i]=(void*)BankTrap;streamVtable[1]=(void*)StreamSet;streamVtable[2]=(void*)StreamGet;
 unsigned count=Next(),cases=0,entries=0,updates=0,overflowCases=0,packs=0,sorted=0;
 for(unsigned c=0;c<count;++c){
  g_flags=Next();g_readOnly=Next();g_chunk=Next();unsigned maxIndex=Next();g_entries=Next();CHECK(g_entries<=64);g_data=Bytes(g_length);
  g_typeCount=Next();g_types=g_words+g_at;g_at+=2*g_typeCount;
  unsigned crcCount=Next(),crcDirty=Next(),crcExtra=Next();const unsigned* crcPairs=g_words+g_at;g_at+=2*crcCount;
  for(unsigned e=0;e<g_entries;++e){Expect& x=g_expect[e];x.index=Next();x.type=Next();x.size=Next();x.offset=Next();x.crc=Next();x.timestamp=Next();x.symbol=Bytes(x.symbolLen);x.names=Next();CHECK(x.names<=8);
   for(unsigned n=0;n<x.names;++n)x.nameBytes[n]=Bytes(x.nameLens[n]);x.name0=x.names?x.nameBytes[0]:0;x.name0Len=x.names?x.nameLens[0]:0;x.info=Bytes(x.infoLen);x.end=Next();}
  unsigned wantCount=Next();const unsigned* want=g_words+g_at;g_at+=2*wantCount;
  memset(bank,0x6b,sizeof(bank));W(bank)[0]=(unsigned)bankVtable;memset(bank+0x38,0,0x40);W(bank+0x78)[0]=g_flags;bank[0x8c]=0;bank[0x8d]=(unsigned char)g_readOnly;bank[0x94]=0;
  unsigned* crc=crcCount+crcExtra?(unsigned*)malloc(8*(crcCount+crcExtra)):0;if(crcCount)memcpy(crc,crcPairs,8*crcCount);W(bank+0xd0)[0]=(unsigned)crc;W(bank+0xd0)[1]=(unsigned)(crc+2*crcCount);W(bank+0xd0)[2]=(unsigned)(crc+2*(crcCount+crcExtra));bank[0xdc]=0;bank[0xdd]=(unsigned char)crcDirty;
  memset(stream,0x5a,sizeof(stream));W(stream)[0]=(unsigned)streamVtable;W(stream)[2]=g_length;Window(0);
  memcpy(before,bank,sizeof(bank));used=0;g_orderCount=0;g_entryIndex=0;g_overflows=0;g_moves=0;memset(g_events,0,sizeof(g_events));
  bool result=ReadList(bank,0,*reinterpret_cast<CFileDataInputStream*>(stream),maxIndex,g_entries);
  CHECK(result&&bank[0x94]==1&&W(stream)[1]==g_length&&g_entryIndex==g_entries&&g_moves==2*g_entries);
  // Event order: allocate, bank size, (init, added) per entry, pack if read-only, post.
  CHECK(g_orderCount==3+2*g_entries+(g_readOnly?1:0)&&g_order[0]==AllocateEvent&&g_order[1]==BankSizeEvent&&g_order[g_orderCount-1]==PostEvent);
  for(unsigned e=0;e<g_entries;++e)CHECK(g_order[2+2*e]==InitEvent&&g_order[3+2*e]==AddedEvent);if(g_readOnly)CHECK(g_order[g_orderCount-2]==PackEvent);
  CHECK(g_size==maxIndex+1);
  // Untouched fields outside the arrays, flags and the CRC map.
  for(unsigned i=0;i<sizeof(bank);++i)if(!(i>=4&&i<0x38)&&i!=0x94&&!(i>=0xd0&&i<0xde))CHECK(bank[i]==before[i]);
  // Each array slot holds the last record written to it.
  CCharString* symbols=(CCharString*)W(bank+8)[0];unsigned* sums=(unsigned*)W(bank+0x14)[0];unsigned char* runtime=(unsigned char*)W(bank+0x20)[0];CBankFileEntryUpdateData** upd=(CBankFileEntryUpdateData**)W(bank+0x2c)[0];
  for(unsigned i=0;i<g_size;++i){int last=-1;for(unsigned e=0;e<g_entries;++e)if(g_expect[e].index==i)last=(int)e;
   if(last<0){CHECK(!W(runtime+12*i)[0]&&!W(runtime+12*i)[1]&&!runtime[12*i+8]&&!runtime[12*i+9]&&Same(symbols[i],(const unsigned char*)"",0)&&!sums[i]&&!upd[i]);continue;}
   Expect& x=g_expect[last];CHECK(W(runtime+12*i)[0]==x.offset&&W(runtime+12*i)[1]==x.size&&runtime[12*i+8]==(unsigned char)x.type&&runtime[12*i+9]==1&&!runtime[12*i+10]&&!runtime[12*i+11]);
   CHECK(g_flags&8?Same(symbols[i],x.symbol,x.symbolLen):Same(symbols[i],(const unsigned char*)"",0));CHECK(sums[i]==(g_flags&16?x.timestamp:0));
   if(g_flags&32){CBankFileEntryUpdateData* u=upd[i];CHECK(u&&u->StateBlockCRC==x.crc&&u->InfoSize==x.infoLen&&(x.infoLen?u->InfoBuffer&&!memcmp(u->InfoBuffer,x.info,x.infoLen):!u->InfoBuffer)&&!u->RequiresUpdate&&u->Exists);
    CHECK(u->Filenames.Size==x.names&&(!x.names||u->Filenames.Data!=0));for(unsigned n=0;n<x.names;++n)CHECK(Same(u->Filenames.Data[n],x.nameBytes[n],x.nameLens[n]));
    delete[] u->InfoBuffer;u->Filenames.Clear();delete u;++updates;}else CHECK(!upd[i]);
  }
  // CRC map: sorted when dirty, then trimmed to its size.
  unsigned* begin=(unsigned*)W(bank+0xd0)[0];unsigned* end=(unsigned*)W(bank+0xd0)[1];unsigned* cap=(unsigned*)W(bank+0xd0)[2];
  CHECK((unsigned)(end-begin)==2*wantCount&&cap==end&&bank[0xdd]==0&&bank[0xdc]==0);for(unsigned i=0;i<2*wantCount;++i)CHECK(begin[i]==want[i]);if(crcDirty&&wantCount>1)++sorted;
  free(begin);for(unsigned i=0;i<g_size;++i)symbols[i].~CCharString();free(symbols);free(sums);free(runtime);free(upd);
  for(unsigned i=0;i<used;++i)CHECK(blocks[i].freed);CHECK(g_CCharStringInstanceCount_013BD800==0);
  entries+=g_entries;overflowCases+=g_overflows>0;packs+=g_readOnly;++cases;
 }
 CHECK(g_at==g_wordCount);free(g_words);
 printf("BANK_ENTRY_LIST_PASS cases=%u entries=%u updates=%u overflow=%u packs=%u sorted=%u\n",cases,entries,updates,overflowCases,packs,sorted);return 0;
}
