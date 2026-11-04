#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_data_input_stream.h"
#define CHECK(x) do {if(!(x)){fprintf(stderr,"INPUT_STREAM_FAILURE line=%d\n",__LINE__);exit(80);}}while(0)
static char emptyText[]="";char* g_FableEmptyNarrowString_0129AAF4=emptyText;
static void* allocation;static unsigned allocations,frees,reads,seeks;
void* operator new[](size_t n) {CHECK(!allocation);allocation=malloc(n);CHECK(allocation);++allocations;return allocation;}
void operator delete[](void* p) {CHECK(p && p==allocation);free(p);allocation=0;++frees;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
extern "C" void ColdBoundary(){fprintf(stderr,"INPUT_STREAM_FAILURE cold boundary\n");exit(80);}
struct File {void** vptr;unsigned access;long position,length;void* handle;bool open;unsigned char padding[3];unsigned pathname;unsigned char data[128];};
static void __fastcall Seek(File* f,void*,long p){CHECK(p>=0 && p<=f->length);f->position=p;++seeks;}
static long __fastcall Position(File* f,void*){return f->position;}
static long __fastcall Length(File* f,void*){return f->length;}
static bool __fastcall IsOpen(File* f,void*){return f->open;}
static unsigned char Byte(File* f,unsigned p){return f->data[p];}
static void __fastcall Read(File* f,void*,void* out,long n,void* process){CHECK(!process && n>=0 && f->position+n<=f->length);for(long i=0;i<n;++i)static_cast<unsigned char*>(out)[i]=Byte(f,f->position+i);f->position+=n;++reads;}
static void CheckRead(CFileDataInputStream& stream,File& file,unsigned n){unsigned char out[256];memset(out,0xcd,sizeof(out));unsigned position=stream.GetPosition();stream.Read(out+8,n);for(unsigned i=0;i<sizeof(out);++i)CHECK(out[i]==(i>=8 && i<8+n?Byte(&file,position+i-8):0xcd));CHECK(stream.GetPosition()==position+n);}
static File* currentFile;static unsigned char bank[0x110];static unsigned entryCalls,recoveryCalls,warnings,stringLive;static bool loaded;
extern "C" bool __fastcall HeaderRead(void*,void*,bool);
#pragma comment(linker,"/alternatename:@HeaderRead@12=?ReadFileInformation@CBankFile@@AAE_N_N@Z")
extern "C" void* __fastcall Filename(void*,void*,void* out){*static_cast<unsigned*>(out)=0;++stringLive;return out;}
extern "C" void* __fastcall CharConstruct(void* out,void*,const char* text,long n){CHECK(text && n==-1);*static_cast<unsigned*>(out)=0;++stringLive;return out;}
extern "C" void __fastcall CharDestroy(void*,void*){CHECK(stringLive);--stringLive;}
extern "C" const char* __fastcall CharPointer(void*,void*){return "fixture";}
extern "C" bool __fastcall Entries(void* self,void*,CFileDataInputStream& stream,unsigned first,unsigned second){CHECK(currentFile && currentFile->position==128);CHECK(self==bank && first==123 && second==456 && stream.GetPosition()==64);CHECK(stream.ReadValue<unsigned long>()==0x41544144);++entryCalls;return loaded;}
extern "C" bool __fastcall Recovery(void* self,void*,unsigned state){CHECK(self==bank && state==2);++recoveryCalls;return loaded;}
static unsigned __fastcall Format(void* self,void*){CHECK(self==bank);return 19;}
static void __fastcall Warning(void*,void*,const void*){++warnings;}
#pragma comment(linker,"/alternatename:?ToCharString@CWideString@@QBE?AVCCharString@@XZ=@Filename@12")
#pragma comment(linker,"/alternatename:??0CCharString@@QAE@PBDJ@Z=@CharConstruct@16")
#pragma comment(linker,"/alternatename:??1CCharString@@QAE@XZ=@CharDestroy@8")
#pragma comment(linker,"/alternatename:??BCCharString@@QBEPBDXZ=@CharPointer@8")
#pragma comment(linker,"/alternatename:?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z=@Entries@20")
#pragma comment(linker,"/alternatename:?ReadRecoveryFile@CBankFile@@AAE_NK@Z=@Recovery@12")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@0@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??H@YI?AVCCharString@@ABV0@PBD@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GFIntToCharString@@YI?AVCCharString@@J@Z=_ColdBoundary")
#include "NativeFileInputStreamColdAliases.h"
extern "C" __declspec(dllimport) void* __stdcall CreateFileA(const char*,unsigned,unsigned,void*,unsigned,unsigned,void*);
extern "C" __declspec(dllimport) int __stdcall WriteFile(void*,const void*,unsigned,unsigned*,void*);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
extern "C" __declspec(dllimport) unsigned __stdcall GetLastError();
extern "C" __declspec(dllimport) unsigned __stdcall SetFilePointer(void*,long,long*,unsigned);
extern "C" void __fastcall DiskRead(File*,void*,void*,long,void*);
extern "C" void __fastcall DiskSeek(File*,void*,long);
extern "C" long __fastcall DiskPosition(File*,void*);
extern "C" void __fastcall DiskClose(File*,void*);
#pragma comment(linker,"/alternatename:@DiskRead@20=?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z")
#pragma comment(linker,"/alternatename:@DiskSeek@12=?SetPosition@CDiskFileWin32@@UAEXJ@Z")
#pragma comment(linker,"/alternatename:@DiskPosition@8=?GetPosition@CDiskFileWin32@@UBEJXZ")
#pragma comment(linker,"/alternatename:@DiskClose@8=?Close@CDiskFileWin32@@UAEXXZ")
#pragma comment(linker,"/alternatename:??0CCharString@@QAE@ABV0@@Z=_ColdBoundary")
int main(int argc,char** argv){CHECK(argc==2);
 void* table[16];for(unsigned i=0;i<16;++i)table[i]=reinterpret_cast<void*>(ColdBoundary);
 table[3]=reinterpret_cast<void*>(DiskRead);table[5]=reinterpret_cast<void*>(DiskSeek);table[7]=reinterpret_cast<void*>(DiskPosition);table[9]=reinterpret_cast<void*>(Length);table[10]=reinterpret_cast<void*>(IsOpen);
 void* bankTable[20];for(i=0;i<20;++i)bankTable[i]=reinterpret_cast<void*>(ColdBoundary);bankTable[8]=reinterpret_cast<void*>(Format);
 void* progressTable[3]={reinterpret_cast<void*>(ColdBoundary),reinterpret_cast<void*>(ColdBoundary),reinterpret_cast<void*>(Warning)};void** progress=progressTable;
 unsigned cases=0;
 for(unsigned ro=0;ro<2;++ro)for(unsigned state=1;state<=2;++state)for(unsigned alignment=16;alignment<=64;alignment+=48)for(unsigned result=0;result<2;++result)for(unsigned error=0;error<4;++error){
  File file;memset(&file,0,sizeof(file));file.vptr=table;file.access=1;file.length=128;file.open=true;
  unsigned header[]={error==1?0:0x42424242,error==2?8:7,error==3?20:19,state,alignment,123,456,64};memcpy(file.data,header,sizeof(header));*reinterpret_cast<unsigned*>(file.data+64)=0x41544144;
  void* writer=CreateFileA(argv[1],0x40000000,0,0,2,0x80,0);CHECK(writer!=reinterpret_cast<void*>(-1));unsigned written=0;CHECK(WriteFile(writer,file.data,128,&written,0) && written==128 && CloseHandle(writer));
  file.handle=CreateFileA(argv[1],0x80000000,1,0,3,0x80,0);CHECK(file.handle!=reinterpret_cast<void*>(-1));
  memset(bank,0,sizeof(bank));*reinterpret_cast<void***>(bank)=bankTable;*reinterpret_cast<File**>(bank+0x7c)=&file;*reinterpret_cast<unsigned*>(bank+0x98)=16;*reinterpret_cast<unsigned*>(bank+0x9c)=7;bank[0x8d]=static_cast<unsigned char>(ro);*reinterpret_cast<void**>(bank+0xec)=&progress;
  currentFile=&file;loaded=result!=0;entryCalls=recoveryCalls=warnings=0;unsigned oldAlloc=allocations,oldFree=frees;
  bool answer=HeaderRead(bank,0,false);bool early=error!=0 || (!ro && alignment!=16);bool expected=error==1?false:error?ro==0:early?true:state==1?loaded:ro==0;
  CHECK(answer==expected && allocations==oldAlloc+1 && frees==oldFree+1 && !allocation && !stringLive);
  CHECK(entryCalls==(!early && state==1?1:0) && recoveryCalls==(!early && !ro && state==2?1:0));
  CHECK(warnings==(!ro && ((error==2 || error==3) || (!error && alignment!=16) || (!error && alignment==16 && state==2 && !loaded))?1:0));
  CHECK(*reinterpret_cast<unsigned*>(bank+0xa0)==(early?0:32) && *reinterpret_cast<unsigned*>(bank+0xa8)==(early?0:128));
  CHECK(*reinterpret_cast<unsigned*>(bank+0x98)==(!error && ro?alignment:16));
  CHECK(file.position==(entryCalls?68:(!error && !early?32:error==1?4:error==2?8:error==3?12:20)));CHECK(SetFilePointer(file.handle,0,0,1)==static_cast<unsigned>(file.position));void* oldHandle=file.handle;DiskClose(&file,0);CHECK(!file.open && !CloseHandle(oldHandle) && GetLastError()==6);++cases;
 }
 printf("NATIVE_HEADER_DISK_PASS cases=%u allocations=%u frees=%u disk_closes=%u\n",cases,allocations,frees,cases);return 0;
}

