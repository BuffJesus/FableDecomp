#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_data_input_stream.h"
#define CHECK(x) do {if(!(x)){fprintf(stderr,"RECOVERY_FAILURE line=%d\n",__LINE__);exit(80);}}while(0)
static void* allocation;static unsigned allocations,frees,strings,opens,entries;
static char emptyText[]="";char* g_FableEmptyNarrowString_0129AAF4=emptyText;
static unsigned char bank[0x110];static bool exists,loaded;static const char* pathname;static void* lastHandle;
struct File {void** table;unsigned access;long position,length;void* handle;bool open;unsigned char padding[3];unsigned path;};
static File* currentFile;
static unsigned fixtureLength,expectedFirst,expectedSecond;
void* operator new[](size_t n){CHECK(!allocation && n==0x4000);allocation=malloc(n);CHECK(allocation);++allocations;return allocation;}
void operator delete[](void* p){CHECK(p==allocation && p);free(p);allocation=0;++frees;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
extern "C" void ColdBoundary(){fprintf(stderr,"RECOVERY_FAILURE cold boundary\n");exit(80);}
extern "C" __declspec(dllimport) void* __stdcall CreateFileA(const char*,unsigned,unsigned,void*,unsigned,unsigned,void*);
extern "C" __declspec(dllimport) int __stdcall WriteFile(void*,const void*,unsigned,unsigned*,void*);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
extern "C" __declspec(dllimport) unsigned __stdcall GetLastError();
extern "C" __declspec(dllimport) unsigned __stdcall SetFilePointer(void*,long,long*,unsigned);
extern "C" bool __fastcall RecoveryRead(void*,void*,unsigned);
#pragma comment(linker,"/alternatename:@RecoveryRead@12=?ReadRecoveryFile@CBankFile@@AAE_NK@Z")
extern "C" void* __fastcall RecoveryPath(void* self,void*,void* out){CHECK(self==bank);*static_cast<unsigned*>(out)=17;++strings;return out;}
extern "C" bool __fastcall PathExists(const void* p){CHECK(*static_cast<const unsigned*>(p)==17);return exists;}
extern "C" void* __fastcall WideConstruct(void* self,void*){*static_cast<unsigned*>(self)=0;++strings;return self;}
extern "C" void __fastcall WideDestroy(void* self,void*){CHECK(strings);--strings;if(*static_cast<unsigned*>(self)==19){CHECK(currentFile && !currentFile->open && !allocation);CHECK(!CloseHandle(lastHandle) && GetLastError()==6);currentFile=0;}}
extern "C" void __fastcall DiskOpen(File* self,void*,const void* path,unsigned access,unsigned flags){CHECK(exists && *static_cast<const unsigned*>(path)==17 && access==1 && flags==4 && !self->open && self->path==0);self->access=access;self->position=0;self->length=fixtureLength;self->handle=CreateFileA(pathname,0x80000000,1,0,3,0x80,0);CHECK(self->handle!=reinterpret_cast<void*>(-1));self->open=true;self->path=19;lastHandle=self->handle;currentFile=self;++opens;}
extern "C" bool __fastcall Entries(void* self,void*,CFileDataInputStream& stream,unsigned first,unsigned second){CHECK(self==bank && currentFile && currentFile->position==static_cast<long>(fixtureLength<0x4000?fixtureLength:0x4000) && first==expectedFirst && second==expectedSecond && stream.GetPosition()==16);CHECK(stream.ReadValue<unsigned long>()==0x41544144 && stream.GetPosition()==20);unsigned n=fixtureLength==128?4:0x4000;unsigned char out[0x4010];memset(out,0xcd,sizeof(out));stream.Read(out+8,n);for(unsigned i=0;i<sizeof(out);++i)CHECK(out[i]==(i>=8 && i<8+n?static_cast<unsigned char>((i-8+20)*37+13):0xcd));CHECK(stream.GetPosition()==20+n);CHECK(SetFilePointer(currentFile->handle,0,0,1)==fixtureLength);++entries;return loaded;}
#pragma comment(linker,"/alternatename:?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ=@RecoveryPath@12")
#pragma comment(linker,"/alternatename:?PathExists@CAFile@@SI_NABVCWideString@@@Z=@PathExists@4")
#pragma comment(linker,"/alternatename:??0CWideString@@QAE@XZ=@WideConstruct@8")
#pragma comment(linker,"/alternatename:??1CWideString@@QAE@XZ=@WideDestroy@8")
#pragma comment(linker,"/alternatename:?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z=@DiskOpen@20")
#pragma comment(linker,"/alternatename:?ReadEntryList@CBankFile@@AAE_NAAVCFileDataInputStream@@KK@Z=@Entries@20")
#pragma comment(linker,"/alternatename:?Write@CDiskFileWin32@@UAEXPBXJPAVCASuspendableProcess@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?MovePosition@CDiskFileWin32@@UAEXJ@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?SetLength@CDiskFileWin32@@UAEXJ@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?Clone@CDiskFileWin32@@UBEPAVCAFile@@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetFileInformation@CAFile@@UAE?AVCFileInformation@@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:??0CWideString@@QAE@ABV0@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??0CCharString@@QAE@PBDJ@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??0CCharString@@QAE@ABV0@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:??1CCharString@@QAE@XZ=_ColdBoundary")
#pragma comment(linker,"/alternatename:??BCCharString@@QBEPBDXZ=_ColdBoundary")
#include "NativeBankRecoveryColdAliases.h"
int main(int argc,char** argv){CHECK(argc==2);pathname=argv[1];unsigned cases=0;
 for(unsigned present=0;present<2;++present)for(unsigned magic=0;magic<2;++magic)for(unsigned valid=0;valid<2;++valid)for(unsigned result=0;result<2;++result)for(unsigned large=0;large<2;++large)for(unsigned pair=0;pair<3;++pair)for(unsigned high=0;high<2;++high){
  fixtureLength=large?0x4020:128;expectedFirst=pair==0?123:pair==1?0:0xffffffff;expectedSecond=pair==0?456:pair==1?0xffffffff:0;unsigned validate=high?0xfedcba98:7;
  unsigned data[0x1008];for(unsigned i=0;i<sizeof(data);++i)reinterpret_cast<unsigned char*>(data)[i]=static_cast<unsigned char>(i*37+13);data[0]=magic?0x43455242:0;data[1]=valid?validate:validate^1;data[2]=expectedFirst;data[3]=expectedSecond;data[4]=0x41544144;
  void* writer=CreateFileA(pathname,0x40000000,0,0,2,0x80,0);CHECK(writer!=reinterpret_cast<void*>(-1));unsigned written=0;CHECK(WriteFile(writer,data,fixtureLength,&written,0) && written==fixtureLength && CloseHandle(writer));
  memset(bank,0,sizeof(bank));exists=present!=0;loaded=result!=0;entries=0;unsigned oldOpen=opens,oldAlloc=allocations,oldFree=frees;bool answer=RecoveryRead(bank,0,validate);
  CHECK(answer==(present && magic && valid) && entries==(present && magic && valid?1:0));CHECK(opens==oldOpen+present && allocations==oldAlloc+present && frees==oldFree+present && !strings && !currentFile && !allocation);++cases;
 }
 printf("NATIVE_RECOVERY_DISK_PASS cases=%u opens=%u allocations=%u frees=%u\n",cases,opens,allocations,frees);return 0;
}
