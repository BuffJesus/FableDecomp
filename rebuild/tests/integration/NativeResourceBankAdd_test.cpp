#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource_bank_add_retail.h"
#include "resource_bank_add_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall BankAdd(void*,void*,void*);
static unsigned arena[4096],initial[4096],expected[4096],location;
static unsigned ptr(unsigned o){return (unsigned)arena+location+o;}
static unsigned& word(unsigned p,unsigned o){return *(unsigned*)(p+o);}
static void require(bool b){if(!b)abort();}
static void model(unsigned bank,unsigned resource){unsigned list=bank+4;word(resource,8)=list;word(resource,24)=word(list,44);word(resource,12)=word(list,16);word(resource,16)=list+4;word(word(resource,12),16)=resource;word(word(resource,16),12)=resource;++word(list,32);word(list,36)+=word(resource,20);}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));const unsigned patterns[6]={0,1,2,0x7fffffff,0x80000000,0xffffffff};unsigned cases=0,snapshots=0;
for(unsigned alias=0;alias<3;++alias)for(unsigned tail=0;tail<3;++tail)for(unsigned size=0;size<6;++size)for(unsigned count=0;count<6;++count)for(unsigned memory=0;memory<6;++memory)for(unsigned age=0;age<6;++age)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){location=place?0x1800:0;unsigned bank=ptr(0x100),resource=alias==1?bank+8:alias==2?bank+64:ptr(0x600);memset(arena,fill?0xa9:0,sizeof(arena));word(bank,20)=tail==0?bank+8:tail==1?resource:ptr(0x900);word(bank,36)=patterns[count];word(bank,40)=patterns[memory];word(bank,48)=patterns[age];word(resource,20)=patterns[size];memcpy(initial,arena,sizeof(arena));model(bank,resource);memcpy(expected,arena,sizeof(arena));
for(unsigned mode=0;mode<2;++mode){memcpy(arena,initial,sizeof(arena));(mode?BankAdd:((void(__fastcall*)(void*,void*,void*))code))((void*)bank,0,(void*)resource);require(!memcmp(expected,arena,sizeof(arena)));++snapshots;}++cases;}
VirtualFree(code,0,0x8000);printf("NATIVE_RESOURCE_BANK_ADD_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
