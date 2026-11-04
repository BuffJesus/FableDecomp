#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "managed_texture_constructor_retail.h"
#include "managed_texture_constructor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void* __fastcall ManagedConstructor(void*,void*);
extern "C" void* __fastcall ManagedScalar(void*,void*,unsigned);
extern "C" unsigned ManagedVTable[];
static unsigned arena[4096],initial[4096],expected[3][4096],captures[3][4096];
static unsigned captured,mutation,ownerOffset;
static void require(bool value){if(!value)abort();}
static void capture(){require(captured<3);memcpy(captures[captured++],arena,sizeof(arena));}
static void event(unsigned id,void* self){require(self==arena+ownerOffset);arena[0x2000/4+id]++;capture();}
extern "C" void* __fastcall BaseBoundary(void* self,void*){
 event(0,self);memset(self,0x73,28);
 if(mutation&1)memset((unsigned char*)self+28,0xb6,48);
 return self;
}
extern "C" void __fastcall DestructorBoundary(void* self,void*){
 event(1,self);if(mutation&1)memset(self,0xda,76);
}
void __cdecl operator delete(void* self){event(2,self);if(mutation&2)((unsigned char*)self)[75]=0xce;}
extern "C" void __fastcall UnusedBoundary(void*,void*){abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 unsigned targets[4]={(unsigned)BaseBoundary,(unsigned)ManagedVTable,(unsigned)DestructorBoundary,(unsigned)&operator delete};
 require(sizeof(fixups)/sizeof(*fixups)==4);
 for(unsigned f=0;f<4;++f){const Fixup& fix=fixups[f];require(fix.target<4);*(unsigned*)(code+fix.site)=fix.kind==20?targets[fix.target]-(unsigned)(code+fix.site+4):targets[fix.target];}
 require(ManagedVTable[0]==(unsigned)ManagedScalar);
 unsigned cases=0,snapshots=0;
 for(unsigned operation=0;operation<2;++operation)for(unsigned profile=0;profile<8;++profile)for(unsigned flagsIndex=0;flagsIndex<8;++flagsIndex)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill)for(mutation=0;mutation<4;++mutation){
  ownerOffset=(place?0x1800:0)/4+0x100/4;unsigned* owner=arena+ownerOffset;unsigned flags=pattern(flagsIndex);
  memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<19;++i)owner[i]=pattern(profile+i);for(unsigned i=0;i<3;++i)arena[0x2000/4+i]=0;memcpy(initial,arena,sizeof(arena));unsigned expectedCount=0;
  for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;void* result;
   if(!mode){
    if(!operation){BaseBoundary(owner,0);owner[0]=(unsigned)ManagedVTable;memset((unsigned char*)owner+28,0,47);}
    else{DestructorBoundary(owner,0);if(flags&1)operator delete(owner);}
    result=owner;
   }else if(!operation)result=(mode==1?((void*(__fastcall*)(void*,void*))code):ManagedConstructor)(owner,0);
   else result=(mode==1?((void*(__fastcall*)(void*,void*,unsigned))(code+128)):ManagedScalar)(owner,0,flags);
   require(result==owner);capture();
   if(!mode){expectedCount=captured;memcpy(expected,captures,sizeof(expected));}
   else{require(captured==expectedCount);for(unsigned i=0;i<captured;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}
  }++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_CONSTRUCTOR_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
