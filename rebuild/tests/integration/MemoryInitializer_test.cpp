#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rebuild_abi.h"
void FABLE_FASTCALL InitializeMemorySystem();
// RETAIL_EXPECTATIONS
static int currentCase,eventIndex;
static unsigned long storage[68];static unsigned char* buffer=reinterpret_cast<unsigned char*>(storage)+4;
static void event(unsigned long a,unsigned long b=0,unsigned long c=0,unsigned long d=0)
{unsigned long values[4]={a,b,c,d};if(eventIndex>=eventCounts[currentCase] || memcmp(values,expected[currentCase][eventIndex],16))abort();++eventIndex;}
extern "C" void* __stdcall Open(const char* name,unsigned long access,unsigned long share,void* security,unsigned long creation,unsigned long flags,void* templ)
{if(strcmp(name,"boot.ini") || access!=0x80000000 || share!=1 || security || creation!=3 || flags!=0x08000000 || templ)abort();event(1,access,share,flags);return reinterpret_cast<void*>(failures[currentCase]?0xffffffff:0x1234);}
extern "C" unsigned long __stdcall Error(){event(2);return 2;}
extern "C" unsigned long __stdcall Size(void* file,unsigned long* high){if(file!=reinterpret_cast<void*>(0x1234)||high)abort();event(3,lengths[currentCase]);return lengths[currentCase];}
extern "C" void* __stdcall Alloc(unsigned int flags,unsigned long size){if(flags!=0x40 || size!=lengths[currentCase])abort();event(4,flags,size);memset(buffer,0,size);memset(buffer-4,0x17,4);memset(buffer+size,0x31,4);return buffer;}
extern "C" int __stdcall Read(void* file,void* target,unsigned long size,unsigned long* count,void* overlapped)
{if(file!=reinterpret_cast<void*>(0x1234)||target!=buffer||size!=lengths[currentCase]||overlapped)abort();unsigned long n=shorts[currentCase]<size?shorts[currentCase]:size;memcpy(target,inputs[currentCase],n);*count=n;event(5,size,n,readOK[currentCase]);return readOK[currentCase];}
extern "C" int __stdcall Close(void* file){if(file!=reinterpret_cast<void*>(0x1234))abort();event(6);return 1;}
extern "C" void* __stdcall Free(void* target){if(target!=buffer)abort();event(8);return 0;}
extern "C" int __cdecl Space(int character){event(7,character);return character==9||character==10||character==11||character==12||character==13||character==32;}
extern "C" {
void* (__stdcall *I0)(const char*,unsigned long,unsigned long,void*,unsigned long,unsigned long,void*)=Open;
unsigned long (__stdcall *I1)()=Error;
unsigned long (__stdcall *I2)(void*,unsigned long*)=Size;
void* (__stdcall *I3)(unsigned int,unsigned long)=Alloc;
int (__stdcall *I4)(void*,void*,unsigned long,unsigned long*,void*)=Read;
int (__stdcall *I5)(void*)=Close;
void* (__stdcall *I6)(void*)=Free;
}
int main()
{
    for(currentCase=0;currentCase<16;++currentCase) {
        eventIndex=0;memset(storage,0xa5,sizeof(storage));InitializeMemorySystem();
        if(eventIndex!=eventCounts[currentCase])return 1;
        if(!failures[currentCase])for(int q=0;q<4;++q)if(buffer[q-4]!=0x17 || buffer[lengths[currentCase]+q]!=0x31)return 2;
    }
    puts("MEMORY_INITIALIZER_OK cases=16");return 0;
}
