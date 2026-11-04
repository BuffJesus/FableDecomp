#include "../../../work/FileInstallerSingletonBase_candidate.h"
#include <stdlib.h>
#include <stdio.h>
#include <new>
static void* object;static void* vtable;static unsigned calls;
extern "C" void __cdecl BaseDelete(void* p){if(p!=object||*(void**)p!=vtable)abort();++calls;}
int main(){
 typedef CTBaseSingleton<CFileInstaller> Base;
 // Fastcall's ECX plus one stack argument matches thiscall; EDX is unused.
 typedef void*(__fastcall* ScalarDelete)(void*,void*,unsigned);
 unsigned cases=0;
 for(unsigned cycle=0;cycle!=32;++cycle)for(unsigned flags=0;flags!=4;++flags){
  object=malloc(sizeof(Base));if(!object)abort();Base* base=::new(object)Base;
  vtable=*(void**)base;ScalarDelete destroy=*(ScalarDelete*)vtable;calls=0;
  if(destroy(base,0,flags)!=base||calls!=(flags&1)||*(void**)base!=vtable)abort();
  free(object);++cases;
 }
 printf("INSTALLER_SINGLETON_BASE_OK cases=%u\n",cases);return 0;
}
