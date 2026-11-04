#include "fable_file_installer.h"
#include "fable_retail_banks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
unsigned char g_FileInstallerVTable_0129B43C;
extern const char g_FileInstallerCompressedExtension_0129B448[]="recorded-compressed";
extern const char g_FileInstallerPlainExtension_0129B440[]="recorded-plain";
void FABLE_CDECL FableInstallerWorker_009C5870(void*){exit(12);}
void FABLE_CDECL FableInstallerDecompressionWorker_009C4420(void*){exit(12);}
static unsigned char* const region=(unsigned char*)0x2004000;
static FILE* input;static unsigned long args[3],calls,allocs,handles,threads;static unsigned char expected[0x8000],snapshot[0x8000],actual[0x8000];static CCharString* temporary;
static void* __stdcall BadEvent(void*,int,int,const wchar_t*){exit(13);return 0;}
extern "C" void*(__stdcall* EventSlot)(void*,int,int,const wchar_t*);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void canonical(){memcpy(actual,region,sizeof(actual));if(*(unsigned long*)actual!=(unsigned long)&g_FileInstallerVTable_0129B43C)exit(10);*(unsigned long*)actual=0x129b43c;}
static void boundary(unsigned long kind,unsigned long a=0,unsigned long b=0,unsigned long c=0){unsigned long event[4];read(event,sizeof(event));read(snapshot,sizeof(snapshot));canonical();if(event[0]!=kind||event[1]!=a||event[2]!=b||event[3]!=c||memcmp(actual,snapshot,sizeof(snapshot))){printf("boundary %lu kind %lu failed\n",calls,kind);exit(9);}++calls;}
void* FABLE_CDECL FableInstallerContainerAllocate(unsigned long size){boundary(1,size,allocs);if(args[2]){*(unsigned long*)(region+0x9c)=0x90000000+allocs;*(unsigned long*)(region+0x74)=17;}return region+0x1000*(++allocs);}
static void __stdcall InitRecording(void* p){if(p!=region+4)exit(11);boundary(2,(unsigned long)p);memset(p,0x5a,24);if(args[2]){region[0x80]=0;region[0x81]=1;region[0x7b]=1;}}
static void* __stdcall EventRecording(void* security,int manual,int initial,const wchar_t* name){if(security||manual||initial||name)exit(11);boundary(3,handles);unsigned long result=args[0]&(1UL<<handles)?0x80000000+handles:0;if(args[2]){region[0x80]=7;region[0x7a]=9;*(unsigned long*)(region+0x84)=0xabc;*(unsigned long*)(region+0x9c)=0xa0000000+handles;}if(args[2]==2&&handles==0)EventSlot=BadEvent;++handles;return(void*)result;}
extern "C"{void(__stdcall* InitSlot)(void*)=InitRecording;void*(__stdcall* EventSlot)(void*,int,int,const wchar_t*)=EventRecording;}
CCharString::CCharString(const char* text,long length){if(text!=g_FileInstallerCompressedExtension_0129B448||length!=-1)exit(11);boundary(4,0x129b448,0xffffffff);temporary=this;storage_=(CCharStringData*)0xfe;}
CCharString& CBasicRetailBankAliasMap::operator[](const CCharString& key){if(this!=(CBasicRetailBankAliasMap*)(region+0x94)||&key!=temporary||*(unsigned long*)&key!=0xfe)exit(11);boundary(5,(unsigned long)this,0xfe);return*(CCharString*)(region+0x6000);}
const CCharString& CCharString::operator=(const char* text){if(this!=(CCharString*)(region+0x6000)||text!=g_FileInstallerPlainExtension_0129B440)exit(11);boundary(6,(unsigned long)this,0x129b440);storage_=(CCharStringData*)0x12345678;return*this;}
CCharString::~CCharString(){if(this!=temporary)exit(11);boundary(7,0xfe);}
extern "C" unsigned long __cdecl ThreadRecording(void(__cdecl* entry)(void*),unsigned int stack,void* parameter){if(entry!=(threads==0?FableInstallerWorker_009C5870:FableInstallerDecompressionWorker_009C4420)||parameter!=region||stack!=0x3000)exit(11);boundary(8,threads==0?0x9c5870:0x9c4420,stack,(unsigned long)parameter);++threads;if(args[2])region[0x81]=1;return args[1];}
int main(int argc,char** argv){if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;if(VirtualAlloc((void*)0x2000000,0x20000,0x3000,4)!=(void*)0x2000000)return 3;unsigned long cases;read(&cases,4);for(unsigned long i=0;i<cases;++i){read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));calls=allocs=handles=threads=0;EventSlot=EventRecording;temporary=0;::new((void*)region)CFileInstaller;canonical();if(calls!=16||allocs!=5||handles!=4||threads!=2||memcmp(actual,expected,sizeof(expected)))return 4;}if(fgetc(input)!=EOF)return 5;printf("FILE_INSTALLER_CTOR_NATIVE_OK cases=%lu\n",cases);return 0;}
