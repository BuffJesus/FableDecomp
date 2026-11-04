#include "fable_boot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
void FABLE_FASTCALL FableDestroyProgressDisplay_00403600(void*);
static unsigned char* const region=(unsigned char*)0x2004000;
static FILE* input;static unsigned long args[5],calls;static unsigned char expected[0x8000],snapshot[0x8000],actual[0x8000];
static void read(void* p,unsigned long n){if(fread(p,1,n,input)!=n)exit(2);}
void* FABLE_CDECL FableOperatorNew(fable_u32 size){if(size!=12||calls>=args[4])exit(8);read(snapshot,sizeof(snapshot));if(memcmp(region,snapshot,sizeof(snapshot)))exit(9);++calls;if(args[2]){*(unsigned long*)region=args[2]==1?0:0x2006000;*(unsigned long*)(region+4)=0x12345678;}return(void*)args[1];}
static unsigned long destroyed;
class TestObject{public:virtual ~TestObject(){++destroyed;}};
static void canonical(){memcpy(actual,region,sizeof(actual));if(args[0]&&args[1]){unsigned long* slot=(unsigned long*)(actual+args[1]-0x2004000+4);if(*slot!=(unsigned long)FableDestroyProgressDisplay_00403600)exit(10);*slot=0x403600;}}
int main(int argc,char** argv){if(argc!=2)return 2;input=fopen(argv[1],"rb");if(!input)return 2;if(VirtualAlloc((void*)0x2000000,0x20000,0x3000,4)!=(void*)0x2000000)return 3;unsigned long cases;read(&cases,4);for(unsigned long i=0;i<cases;++i){read(args,sizeof(args));read(region,sizeof(expected));read(expected,sizeof(expected));calls=0;::new((void*)region) CCountedProgressDisplay((CProgressDisplay*)args[0]);canonical();if(calls!=args[4]||memcmp(actual,expected,sizeof(expected))){printf("case %lu failed\n",i);return 4;}}if(fgetc(input)!=EOF)return 5;FableDestroyProgressDisplay_00403600(0);if(destroyed)return 6;TestObject* p=new TestObject;FableDestroyProgressDisplay_00403600(p);if(destroyed!=1)return 6;printf("PROGRESS_COUNTED_NATIVE_OK cases=%lu destroy_cases=2\n",cases);return 0;}
