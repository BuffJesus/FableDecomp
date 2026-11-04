#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_engine_primitive_handle.h"
#include "fable_engine_internal.h"
#include "fable_engine_primitive_manager.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineScene.h"
#include "primitive_handle_clear_retail.h"
#include "primitive_handle_clear_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall HandleClear(void*,void*);
static unsigned arena[4096],initial[4096],images[3][4096],mode,steps,count,snapshots,location,notifyMutation,deleteMutation,notifyCalls,deleteCalls;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned p,unsigned off){return *(unsigned*)(p+off);}
static void require(bool b){if(!b)abort();}
// Link-only dependencies of the existing renderer TU which emits Clear's
// inline COMDAT. This leaf fixture must never enter the renderer parent.
namespace NEngineInternal {CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo=0;CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D=0;}
void CEnginePrimitiveManager::AddPrimitive(CEngineInternalPrimitiveBase&,unsigned long){abort();}
void CEnginePrimitiveRenderer2D::AddPrimitive(CEngineInternalPrimitiveBase&){abort();}
void FABLE_FASTCALL CEngineScene::UpdatePosition(CEngineInternalPrimitiveBase&,unsigned long){abort();}
static void snapshot(){if(mode==0){require(count<3);memcpy(images[count++],arena,sizeof(arena));}else{require(steps<count&&!memcmp(images[steps++],arena,sizeof(arena)));++snapshots;}}
static void notify(unsigned object,unsigned handle){require(object==ptr(0x400)&&handle==ptr(0x100));++notifyCalls;word(ptr(0x300),0)=notifyCalls;if(notifyMutation==1)word(handle,4)=0;if(notifyMutation==2)word(handle,4)=ptr(0x800);if(notifyMutation==3)word(object,4)=0;snapshot();}
static void destroy(unsigned object){require(object==ptr(0x400)||object==ptr(0x800));++deleteCalls;word(ptr(0x300),4)=deleteCalls;word(object,8)=0xdededede;if(deleteMutation)word(ptr(0x100),4)=ptr(0x800);snapshot();}
class Boundaries {public:void Notify(const CEnginePrimitiveHandle& handle);void Delete()const;};
void Boundaries::Notify(const CEnginePrimitiveHandle& handle){notify((unsigned)this,(unsigned)&handle);}
void Boundaries::Delete()const{destroy((unsigned)this);}
static void model(){unsigned h=ptr(0x100),p=word(h,4);if(p&&word(p,4)==1)notify(p,h);p=word(h,4);if(p){if(--word(p,4)==0)destroy(p);word(h,4)=0;}word(h,4)=0;snapshot();}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 void(Boundaries::*d)()const=&Boundaries::Delete;void(Boundaries::*n)(const CEnginePrimitiveHandle&)=&Boundaries::Notify;unsigned table[4]={0,0,0,0};memcpy(&table[1],&d,4);memcpy(&table[3],&n,4);const unsigned refs[6]={0,1,2,3,0x80000000,0xffffffff};unsigned cases=0;
 for(unsigned live=0;live<2;++live)for(unsigned ref=0;ref<6;++ref)for(unsigned altRef=0;altRef<3;++altRef)for(notifyMutation=0;notifyMutation<4;++notifyMutation)for(deleteMutation=0;deleteMutation<2;++deleteMutation)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 location=place?0x1800:0;memset(arena,fill?0xa9:0,sizeof(arena));word(ptr(0x100),4)=live?ptr(0x400):0;word(ptr(0x400),0)=word(ptr(0x800),0)=(unsigned)table;word(ptr(0x400),4)=refs[ref];word(ptr(0x800),4)=refs[altRef];word(ptr(0x300),0)=word(ptr(0x300),4)=0;memcpy(initial,arena,sizeof(arena));count=0;
 unsigned expectedNotify=0,expectedDelete=0;for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));steps=notifyCalls=deleteCalls=0;if(mode==0){model();expectedNotify=notifyCalls;expectedDelete=deleteCalls;}else{(mode==1?((void(__fastcall*)(void*,void*))code):HandleClear)((void*)ptr(0x100),0);snapshot();require(steps==count&&notifyCalls==expectedNotify&&deleteCalls==expectedDelete);}}
 ++cases;
 }VirtualFree(code,0,0x8000);printf("NATIVE_PRIMITIVE_HANDLE_CLEAR_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
