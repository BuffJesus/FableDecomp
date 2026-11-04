typedef unsigned long DWORD;
typedef void* HANDLE;
enum {FALSE=0,WAIT_OBJECT_0=0,ERROR_INVALID_HANDLE=6};
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(void*);
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) HANDLE __stdcall CreateEventA(void*,int,int,const char*);
__declspec(dllimport) HANDLE __stdcall CreateThread(void*,unsigned long,DWORD(__stdcall*)(void*),void*,unsigned long,unsigned long*);
__declspec(dllimport) DWORD __stdcall WaitForSingleObject(HANDLE,DWORD);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE);
__declspec(dllimport) int __stdcall GetHandleInformation(HANDLE,DWORD*);
__declspec(dllimport) DWORD __stdcall GetLastError();
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_file_installer_active_files.h"
#include "fable_retail_banks.h"
static void require(bool ok){if(!ok)abort();}
static unsigned clearCalls,listCalls;
struct Worker {unsigned char* bytes;HANDLE event;unsigned flag;};
static DWORD __stdcall FinishWorker(void* p){
 Worker& worker=*static_cast<Worker*>(p);require(WaitForSingleObject(worker.event,10000)==WAIT_OBJECT_0);
 void* cs=worker.bytes+4;EnterCriticalSection(cs);
 require(worker.bytes[0x7a]==1);worker.bytes[worker.flag]=1;LeaveCriticalSection(cs);return 0;
}
static unsigned live,baseCalls,destroyed;
static void* __cdecl Allocate(void*,unsigned count,unsigned width){void* p=calloc(count,width);require(p!=0);++live;return p;}
static void __cdecl Release(void*,void* p){if(p){require(live!=0);--live;free(p);}}
void CCompressorZlib::Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::SetQuality(float){abort();}
extern "C" void __fastcall BDtor(void*){++baseCalls;}
static void FABLE_FASTCALL Destroy(void* p){require(live==0);++destroyed;delete static_cast<unsigned*>(p);}
static void transfer(InstallerTransferList& list,unsigned count){
 typedef InstallerTransferList::Link Link;
 Link* head=static_cast<Link*>(malloc(sizeof(Link)));require(head!=0);list.head=head;Link* last=head;
 for(unsigned i=0;i<count;++i){Link* link=static_cast<Link*>(malloc(sizeof(Link)));require(link!=0);last->next=link;link->previous=last;last=link;}
 last->next=head;head->previous=last;if(!count)head->next=head;
}

char g_FableEmptyNarrowString_0129AAF4[1]={0};
void CFileInstaller::ClearBuffers(){++clearCalls;}
void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk* first,InstallerBufferChunk* last,void*){require(!first&&!last);}
int main(){
 unsigned cases=0,threads=0;
 for(unsigned cycle=0;cycle!=32;++cycle)for(unsigned mode=0;mode!=3;++mode){
  unsigned long storage[sizeof(CFileInstaller)/4];memset(storage,0,sizeof(storage));unsigned char* bytes=reinterpret_cast<unsigned char*>(storage);
  InitializeCriticalSection(bytes+4);
  CCharString retained("retained-installer-owner",-1);long baseline=g_CCharStringInstanceCount_013BD800;
  unsigned* payload=new unsigned(123);CCPPointerInfo* control=new CCPPointerInfo(Destroy,payload);control->RefCount=3;
  for(unsigned off=0x38;off<=0x3c;off+=4){
   InstallerCountedFileList& list=*reinterpret_cast<InstallerCountedFileList*>(bytes+off);
   list.head=static_cast<InstallerCountedFileList::Node*>(malloc(16));require(list.head!=0);
   InstallerCountedFileList::Node* n=static_cast<InstallerCountedFileList::Node*>(malloc(16));require(n!=0);
   ::new(&n->value)InstallerCountedFileList::Value();n->value.Data=reinterpret_cast<const CFileInstaller::CFileInfo*>(payload);n->value.Info=control;
   list.head->next=list.head->previous=n;n->next=n->previous=list.head;
  }
  InstallerActiveFileList& active=*reinterpret_cast<InstallerActiveFileList*>(bytes+0x40);
  active.head=static_cast<InstallerActiveFileList::Node*>(malloc(sizeof(InstallerActiveFileList::Node)));require(active.head!=0);
  InstallerActiveFileList::Node* a=static_cast<InstallerActiveFileList::Node*>(calloc(1,sizeof(InstallerActiveFileList::Node)));require(a!=0);
  ::new(&a->file)InstallerActiveFileList::Value();a->file.Data=reinterpret_cast<const CFileInstaller::CFileInfo*>(payload);a->file.Info=control;
  active.head->next=active.head->previous=a;a->next=a->previous=active.head;transfer(a->input,cycle%3);transfer(a->output,(cycle+1)%3);
  if(cycle%3){a->compressor.Stream.zalloc=Allocate;a->compressor.Stream.zfree=Release;int status=cycle%3==1?z_deflateInit_(&a->compressor.Stream,6,"1.2.1",sizeof(z_stream_s)):z_inflateInit_(&a->compressor.Stream,"1.2.1",sizeof(z_stream_s));require(status==0);a->compressor.ZlibInitialised=true;a->compressor.Compressing=cycle%3==1;}
  InstallerCompletedFileSet& completed=*reinterpret_cast<InstallerCompletedFileSet*>(bytes+0x44);
  completed.head=static_cast<InstallerCompletedFileSet::Node*>(calloc(1,20));InstallerCompletedFileSet::Node* c=static_cast<InstallerCompletedFileSet::Node*>(calloc(1,20));require(completed.head&&c);::new(&c->value)CCharString(retained);completed.head->parent=c;completed.size=1;
  CBasicRetailBankAliasMap& aliases=*reinterpret_cast<CBasicRetailBankAliasMap*>(bytes+0x94);
  aliases.head_=static_cast<CBasicRetailBankAliasMap::CNode*>(calloc(1,24));CBasicRetailBankAliasMap::CNode* b=static_cast<CBasicRetailBankAliasMap::CNode*>(calloc(1,24));require(aliases.head_&&b);::new(&b->value_)CBasicRetailBankAliasMap::value_type(retained,retained);aliases.head_->parent_=b;aliases.size_=1;
  HANDLE events[4];for(unsigned i=0;i!=4;++i){events[i]=CreateEventA(0,FALSE,FALSE,0);require(events[i]!=0);*reinterpret_cast<HANDLE*>(bytes+0x84+i*4)=events[i];}
  bytes[0x78]=mode==0;bytes[0x79]=mode!=2;
  Worker workers[2];HANDLE running[2]={0,0};
  for(i=0;i!=mode;++i){workers[i].bytes=bytes;workers[i].event=events[i];workers[i].flag=0x78+i;running[i]=CreateThread(0,0,FinishWorker,&workers[i],0,0);require(running[i]!=0);++threads;}
  reinterpret_cast<CFileInstaller*>(bytes)->CFileInstaller::~CFileInstaller();
  require(bytes[0x78]&&bytes[0x79]&&bytes[0x7a]&&clearCalls==cases+1&&live==0&&destroyed==cases+1&&baseCalls==cases+1&&g_CCharStringInstanceCount_013BD800==baseline);
  for(i=0;i!=mode;++i){require(WaitForSingleObject(running[i],10000)==WAIT_OBJECT_0);require(CloseHandle(running[i])!=0);}
  for(i=0;i!=4;++i){DWORD flags;require(!GetHandleInformation(events[i],&flags)&&GetLastError()==ERROR_INVALID_HANDLE);}
  ++cases;
 }
 printf("INSTALLER_DESTRUCTOR_CONTAINERS_OK cases=%u fixture_threads=%u events=%u\n",cases,threads,cases*4);return 0;
}
