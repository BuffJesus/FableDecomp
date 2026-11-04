#include <new>
#include <stdio.h>
#include <string.h>
#include "fable_render_state_manager.h"
static unsigned long storage[0x3d20/4],expected[0x3d20/4];
static int failures,mutation,events[12],count,mode,valid;
static CRenderStateManager* State(){return reinterpret_cast<CRenderStateManager*>(storage);}
static CRenderManagerCore* Core(){return reinterpret_cast<CRenderManagerCore*>(storage);}
static void check(bool b){if(!b)++failures;}
static void event(int e){events[count++]=e;}
static void set(unsigned offset,unsigned long value){expected[offset/4]=value;}
static void byte(unsigned offset,unsigned char value){reinterpret_cast<unsigned char*>(expected)[offset]=value;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){
 check(this==State());event(10);
 if(mutation==1){storage[1]=0x12345678;storage[0x2824/4]=0x10203040;reinterpret_cast<unsigned char*>(storage)[0x3a36]=0x81;set(4,0x12345678);set(0x2824,0x10203040);byte(0x3a36,0x81);}
}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CRenderStateManager::~CRenderStateManager(){check(this==State());event(50);}
CInitBaseClass::CInitBaseClass():Valid(false){
 check(this==&Core()->SystemFont);event(20);
 if(mutation==2){Core()->DisplayManager=reinterpret_cast<CDisplayManager*>(0x222200);set(0x3a3c,0x222200);Core()->CurrentlySelectedTextures[0]=reinterpret_cast<IDirect3DBaseTexture9*>(0x444400);}
}
CInitBaseClass::~CInitBaseClass(){check(this==&Core()->SystemFont);event(40);}
void CInitBaseClass::Validate() const {}
bool CInitBaseClass::IsValid() const {
 check(this==&Core()->SystemFont);event(30);bool old=Valid;
 if(mutation==1){reinterpret_cast<unsigned char*>(&Core()->SystemFont)[4]=0;byte(0x3a50,0);}return old;
}
void CSystemFont::Uninitialise(){check(this==&Core()->SystemFont);event(35);if(mutation==2){Core()->PD3DDevice=reinterpret_cast<IDirect3DDevice9*>(0x334400);set(0x3cf0,0x334400);}}
static void record(unsigned base,unsigned count){
 for(unsigned i=0;i<count;++i){unsigned at=base+20*i;set(at,0);set(at+4,0);set(at+8,0);byte(at+16,0);byte(at+17,0);}
}
int main(){unsigned scenarios=0;const unsigned long flags[]={0,1,0x80000000,0xffffffff};
 for(unsigned fill=0;fill<256;++fill)for(unsigned f=0;f<4;++f)for(mutation=0;mutation<3;++mutation)for(mode=0;mode<3;++mode){
  memset(storage,fill,sizeof(storage));memcpy(expected,storage,sizeof(storage));count=0;
  if(mode==0){CRenderStateManager::CDeviceState* result=new(storage) CRenderStateManager::CDeviceState;check(result==reinterpret_cast<CRenderStateManager::CDeviceState*>(storage));record(0,231);check(count==0);}
  else {
   // Font-constructor callbacks can replace the display pointer after its initialization.
   if(mode==2)set(0x3a3c,0x111100);
   if(mode==1){CRenderStateManager* result=new(storage) CRenderStateManager(flags[f]);check(result==State());}
   else {CRenderManagerCore* result=new(storage) CRenderManagerCore(*reinterpret_cast<CDisplayManager*>(0x111100),flags[f]);check(result==Core());}
   check(count==(mode==1?1:2) && events[0]==10 && (mode==1||events[1]==20));
   set(0,storage[0]);record(0x2818,231);record(0x3a24,1);byte(0x3a35,4);set(0x2808,0);set(0x280c,0);set(0x2810,0);set(0x2814,1);set(0x3a38,flags[f]);
   if(mode==2){set(0x3a48,0xffffffff);set(0x3a4c,storage[0x3a4c/4]);byte(0x3a50,0);for(unsigned i=0;i<8;++i)set(0x3d00+4*i,0);}
  }
  check(memcmp(storage,expected,sizeof(storage))==0);++scenarios;
 }
 for(unsigned fill=0;fill<256;++fill)for(valid=0;valid<2;++valid)for(mutation=0;mutation<3;++mutation){
  memset(storage,fill,sizeof(storage));reinterpret_cast<unsigned char*>(storage)[0x3a50]=valid;memcpy(expected,storage,sizeof(storage));count=0;
  Core()->CRenderManagerCore::~CRenderManagerCore();
  int want[]={30,35,40,50};check(count==(valid?4:3));check(events[0]==30 && events[count-2]==40 && events[count-1]==50);if(valid)check(events[1]==35);
  set(0,storage[0]);set(0x3a4c,storage[0x3a4c/4]);check(memcmp(storage,expected,sizeof(storage))==0);++scenarios;
 }
 printf("RENDER_CONSTRUCTORS_BEHAVIOR_%s scenarios=%u failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
