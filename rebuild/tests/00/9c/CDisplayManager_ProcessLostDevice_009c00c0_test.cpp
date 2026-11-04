#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[0x2c8/4],deviceTable[119],device;
static unsigned errors,callCount,mutation,results[3];
static HRESULT cooperative;
static unsigned char snapshots[3][44];
static bool retained[3];
static CDisplayManager* Object(){return reinterpret_cast<CDisplayManager*>(storage);}
static void Write(unsigned char* bytes,unsigned offset,unsigned value){memcpy(bytes+offset,&value,4);}
static long Read(const unsigned char* bytes,unsigned offset){long value;memcpy(&value,bytes+offset,4);return value;}
static void Mutate(unsigned char* bytes,unsigned call){
 if(mutation==1 && call==0){Write(bytes,4,2048);Write(bytes,8,900);Write(bytes,20,3);Write(bytes,40,4);}
 if(mutation==2 && call==1){Write(bytes,4,4096);Write(bytes,8,1200);Write(bytes,24,30);Write(bytes,40,4);}
}
static void Clamp(unsigned char* bytes){if(Read(bytes,4)>1024)Write(bytes,4,1024);if(Read(bytes,8)>768)Write(bytes,8,768);}
static HRESULT FABLE_STDCALL Cooperative(IDirect3DDevice9* self){if(self!=reinterpret_cast<IDirect3DDevice9*>(&device))++errors;return cooperative;}
CDisplayManager::ECreateDeviceSuccess CDisplayManager::ResetDevice(const CCreateDeviceParameters& parameters){
 if(this!=Object() || callCount>=3){++errors;return CREATE_DEVICE_FAILED;}
 retained[callCount]=&parameters==&LastDeviceCreationParameters;memcpy(snapshots[callCount],&parameters,44);Mutate(reinterpret_cast<unsigned char*>(const_cast<CCreateDeviceParameters*>(&parameters)),callCount);return static_cast<ECreateDeviceSuccess>(results[callCount++]);
}
static bool Same(const unsigned char* a,const unsigned char* b){return !memcmp(a,b,29) && !memcmp(a+32,b+32,12);}
static void Run(long width,long height,unsigned sample){
 memset(storage,0,sizeof(storage));Object()->D3DDevice=reinterpret_cast<IDirect3DDevice9*>(&device);
 CDisplayManager::CCreateDeviceParameters& p=Object()->LastDeviceCreationParameters;memset(&p,0xa7,44);p.BackBufferDimensions.X=width;p.BackBufferDimensions.Y=height;p.MultisampleType=static_cast<ESurfaceMultisampleType>(sample);p.ExclusiveMode=true;p.PresentationInterval.PresentImmediate=false;p.PresentationInterval.PresentImmediateIfSyncMissed=true;
 unsigned char expectedLast[44],expected[3][44];memcpy(expectedLast,&p,44);memcpy(expected[0],&p,44);callCount=0;
 bool actual=Object()->ProcessLostDevice(),wanted=cooperative==0;unsigned count=0;
 if(cooperative==static_cast<HRESULT>(0x88760869L)){
  count=1;Mutate(expectedLast,0);wanted=results[0]==2;
  if(results[0]!=1 && results[0]!=2){
   memcpy(expected[1],expectedLast,44);
   if(Read(expected[1],40)){
    Write(expected[1],40,0);count=2;wanted=results[1]==2;
    if(results[1]!=1 && results[1]!=2){memcpy(expected[2],expected[1],44);Mutate(expected[2],1);Clamp(expected[2]);count=3;wanted=results[2]==2;}
   }else{Clamp(expected[1]);count=2;wanted=results[1]==2;}
  }
 }
 if(actual!=wanted || callCount!=count || memcmp(expectedLast,&p,44))++errors;
 for(unsigned i=0;i<count;++i)if(retained[i]!=(i==0) || !Same(snapshots[i],expected[i]))++errors;
}
int main(){deviceTable[3]=reinterpret_cast<unsigned long>(&Cooperative);device=reinterpret_cast<unsigned long>(deviceTable);unsigned scenarios=0;const long dimensions[][2]={{640,480},{1920,1080},{-1,3},{1025,767},{1024,768}};const unsigned codes[]={0,1,2,7};
 cooperative=static_cast<HRESULT>(0x88760869L);
 for(unsigned dim=0;dim<5;++dim)for(unsigned ms=0;ms<2;++ms)for(unsigned a=0;a<4;++a)for(unsigned b=0;b<4;++b)for(unsigned c=0;c<4;++c)for(mutation=0;mutation<3;++mutation){results[0]=codes[a];results[1]=codes[b];results[2]=codes[c];Run(dimensions[dim][0],dimensions[dim][1],ms*2);++scenarios;}
 const HRESULT hr[]={0,1,D3DERR_DEVICELOST,static_cast<HRESULT>(0x80004005)};
 for(unsigned h=0;h<4;++h)for(unsigned sample=0;sample<4;++sample){cooperative=hr[h];Run(1920,1080,sample*2);++scenarios;}
 if(errors){printf("DISPLAY_LOST_FAIL errors=%u\n",errors);return 1;}printf("DISPLAY_LOST_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
