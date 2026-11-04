#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned expectedSize,allocations,releases;
static unsigned char* allocation;
static void require(bool ok,int line){if(!ok){fprintf(stderr,"BYTE_VECTOR_CTOR_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
extern "C" void* __cdecl FixtureVectorCtorMalloc(size_t n){
 CHECK(n==expectedSize && n && !allocation);unsigned char* raw=(unsigned char*)malloc(n+32);CHECK(raw);memset(raw,0x5a,n+32);allocation=raw+16;++allocations;return allocation;
}
extern "C" void __cdecl FixtureVectorCtorFree(void* p){
 if(!p)return;CHECK(p==allocation);for(unsigned i=0;i<16;++i)CHECK(allocation[int(i)-16]==0x5a && allocation[expectedSize+i]==0x5a);free(allocation-16);allocation=0;++releases;
}
#define free FixtureVectorCtorFree
#include "fable_stlport_vector.h"
#undef free
struct FakeDisk{void** vptr;unsigned char fields[24];};
static FakeDisk* diskExpected;static unsigned scalarCalls;
static void* __fastcall Scalar(FakeDisk* disk,void*,unsigned flags){CHECK(disk==diskExpected && flags==1);++scalarCalls;return disk;}
extern "C" void __fastcall RetailDiskDelete(void*);
#pragma comment(linker,"/alternatename:@RetailDiskDelete@4=?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@SIXPAX@Z")
int main(){
 unsigned boundaries[]={0,1,2,3,4,7,15,16,17,31,32,33,63,64,65,127,128,255,256,257,4095,4096,4097,16383,16384,16385,65535,65536};
 unsigned state=0x12345678;
 for(unsigned test=0;test<256;++test){state=state*1664525+1013904223;expectedSize=test<sizeof(boundaries)/sizeof(boundaries[0])?boundaries[test]:state%65537;
  unsigned words[16];memset(words,0xa7,sizeof(words));unsigned beforeAlloc=allocations,beforeFree=releases;
  _STL::vector<unsigned char>* value=new(words+4) _STL::vector<unsigned char>(expectedSize);
  CHECK(value->size()==expectedSize && value->capacity()==expectedSize && value->begin()==allocation);
  CHECK(allocations==beforeAlloc+(expectedSize!=0));for(unsigned i=0;i<expectedSize;++i)CHECK((*value)[i]==0);
  value->~vector();CHECK(!allocation && releases==beforeFree+(expectedSize!=0));
  for(unsigned k=0;k<sizeof(words);++k)if(k<16 || k>=28)CHECK(((unsigned char*)words)[k]==0xa7);
 }
 void* table[1]={(void*)Scalar};FakeDisk disk;memset(&disk,0x6c,sizeof(disk));disk.vptr=table;diskExpected=&disk;RetailDiskDelete(&disk);CHECK(scalarCalls==1 && disk.vptr==table);for(unsigned k=0;k<sizeof(disk.fields);++k)CHECK(disk.fields[k]==0x6c);
 CHECK(allocations==releases);printf("NATIVE_BYTE_VECTOR_CTOR_PASS cases=256 callback=1\n");return 0;
}
