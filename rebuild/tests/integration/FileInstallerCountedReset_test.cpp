#include "fable_file_installer.h"
#include "fable_boot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
void FABLE_FASTCALL FableDestroyFileInstaller_00403630(void*);
static unsigned char* const region=(unsigned char*)0x2004000;
static FILE* input;static unsigned long args[6],calls;static unsigned char expected[0x8000],snapshot[0x8000],actual[0x8000];
static void FABLE_FASTCALL DestroyRecording(void*);
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
static void canonical(){memcpy(actual,region,sizeof(actual));for(unsigned long offset=0x3004;offset<=0x5004;offset+=0x2000){unsigned long* slot=(unsigned long*)(actual+offset);if(*slot!=(unsigned long)DestroyRecording)exit(10);*slot=0x200d200;}if(args[0]&&args[3]&&calls==args[5]){unsigned long* slot=(unsigned long*)(actual+0x4004);if(*slot!=(unsigned long)FableDestroyFileInstaller_00403630)exit(11);*slot=0x403630;}}
static void boundary(unsigned long kind,unsigned long parameter){if(calls>=args[5])exit(8);unsigned long event[2];read(event,sizeof(event));read(snapshot,sizeof(snapshot));canonical();if(event[0]!=kind||event[1]!=parameter||memcmp(actual,snapshot,sizeof(snapshot))){printf("event %lu kind %lu failed\n",calls,kind);exit(9);}++calls;}
static void FABLE_FASTCALL DestroyRecording(void* p){boundary(1,(unsigned long)p);if(args[4]){*(unsigned long*)(region+4)=0x2009000;*(unsigned long*)region=0x2006000;}}
void FABLE_CDECL FableOperatorDelete(void* p){boundary(2,(unsigned long)p);if(args[4]==2){*(unsigned long*)(region+4)=0x2007000;*(unsigned long*)region=0;}}
void* FABLE_CDECL FableOperatorNew(fable_u32 size){boundary(3,size);if(size!=12)exit(12);if(args[4]){*(unsigned long*)region=args[4]==1?0:0x2006000;*(unsigned long*)(region+4)=0x12345678;}return(void*)args[3];}
static unsigned long destroyed;
class TestObject{public:virtual ~TestObject(){++destroyed;}};
int main(int argc,char** argv){if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;if(VirtualAlloc((void*)0x2000000,0x20000,0x3000,4)!=(void*)0x2000000)return 3;unsigned long cases;read(&cases,4);for(unsigned long i=0;i<cases;++i){read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));*(unsigned long*)(region+0x3004)=(unsigned long)DestroyRecording;*(unsigned long*)(region+0x5004)=(unsigned long)DestroyRecording;calls=0;*(CCountedFileInstaller*)region=(CFileInstaller*)args[0];canonical();if(calls!=args[5]||memcmp(actual,expected,sizeof(expected))){printf("case %lu failed\n",i);return 4;}}if(fgetc(input)!=EOF)return 5;FableDestroyFileInstaller_00403630(0);if(destroyed)return 6;TestObject* p=new TestObject;FableDestroyFileInstaller_00403630(p);if(destroyed!=1)return 6;printf("FILE_INSTALLER_RESET_NATIVE_OK cases=%lu destroy_cases=2\n",cases);return 0;}
