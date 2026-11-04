#include "fable_surface.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[0x2c8/4];
static unsigned char expectedObject[0x2c8];
static unsigned errors,colourCalls,resetCalls,mutation,resultCode,scenarios;
static bool exclusiveTest,requestedExclusive;
static C2DExtentsI requestedDimensions;
static unsigned long requestedDepth,requestedRefresh;
static ESurfaceMultisampleType requestedMultisample;
static CDisplayManager* Object(){return reinterpret_cast<CDisplayManager*>(storage);}
static long Clamp(long n){return n<32?32:n;}
unsigned long CPixelFormat::GetColourDepth() const {
 if(this!=reinterpret_cast<CPixelFormat*>(&Object()->BackBufferFormatStorage_))++errors;
 ++colourCalls;
 if(mutation){Object()->LastDeviceCreationParameters.ZBufferBitDepth=91;Object()->LastDeviceCreationParameters.BackBufferCount=7;Object()->LastDeviceCreationParameters.ExclusiveMode=!Object()->LastDeviceCreationParameters.ExclusiveMode;Object()->LastDeviceCreationParameters.PresentationInterval.VSyncCount=9;memcpy(expectedObject,Object(),sizeof(storage));}
 return 32;
}
CDisplayManager::ECreateDeviceSuccess CDisplayManager::ResetDevice(const CCreateDeviceParameters& p){
 if(this!=Object())++errors;++resetCalls;
 unsigned char expected[44];memcpy(expected,&LastDeviceCreationParameters,44);
 if(exclusiveTest)expected[28]=requestedExclusive?1:0;
 else{long x=Clamp(requestedDimensions.X),y=Clamp(requestedDimensions.Y);memcpy(expected+4,&x,4);memcpy(expected+8,&y,4);memcpy(expected+12,&requestedDepth,4);memcpy(expected+24,&requestedRefresh,4);memcpy(expected+40,&requestedMultisample,4);}
 // Three outer padding bytes are uninitialized in the native copy constructor.
 if(memcmp(&p,expected,29) || memcmp(reinterpret_cast<const char*>(&p)+32,expected+32,12))++errors;
 return static_cast<ECreateDeviceSuccess>(resultCode);
}
static void Fill(unsigned pattern){
 memset(storage,static_cast<unsigned char>(pattern),sizeof(storage));Object()->ExclusiveMode=(pattern&1)!=0;
 CDisplayManager::CCreateDeviceParameters& p=Object()->LastDeviceCreationParameters;
 p.ExclusiveMode=(pattern&2)!=0;p.PresentationInterval.PresentImmediate=(pattern&4)!=0;p.PresentationInterval.PresentImmediateIfSyncMissed=(pattern&8)!=0;
 Object()->RefreshRate=60;Object()->MultisampleType=SURFACE_MULTISAMPLE_2;
 colourCalls=resetCalls=0;
 memcpy(expectedObject,storage,sizeof(storage));
}
int main(){
 const long dimensions[]={-2147483647L-1,0,31,32,2147483647L};
 exclusiveTest=false;
 for(unsigned x=0;x<5;++x)for(unsigned y=0;y<5;++y)for(unsigned variation=0;variation<16;++variation)for(mutation=0;mutation<2;++mutation)for(resultCode=0;resultCode<3;++resultCode){
  Fill(x*11+y*7+variation);requestedDimensions.X=dimensions[x];requestedDimensions.Y=dimensions[y];
  Object()->BackBufferDimensionsStorage_[0]=(variation&1)?64:requestedDimensions.X;Object()->BackBufferDimensionsStorage_[1]=requestedDimensions.Y;
  requestedDepth=(variation&2)?16:32;requestedRefresh=(variation&4)?75:60;requestedMultisample=(variation&8)?SURFACE_MULTISAMPLE_4:SURFACE_MULTISAMPLE_2;
  memcpy(expectedObject,storage,sizeof(storage));unsigned result=Object()->SetDisplayMode(requestedDimensions,requestedDepth,requestedMultisample,requestedRefresh);
  if(colourCalls!=((variation&1)?0U:1U) || resetCalls!=(variation?1U:0U) || result!=(variation?resultCode:2U) || memcmp(expectedObject,storage,sizeof(storage)))++errors;
  ++scenarios;
 }
 exclusiveTest=true;
 for(unsigned pattern=0;pattern<8;++pattern)for(unsigned current=0;current<2;++current)for(unsigned requested=0;requested<2;++requested)for(resultCode=0;resultCode<3;++resultCode){
  Fill(pattern*31);Object()->ExclusiveMode=current!=0;requestedExclusive=requested!=0;memcpy(expectedObject,storage,sizeof(storage));unsigned result=Object()->SetExclusiveMode(requestedExclusive);
  if(colourCalls || resetCalls!=(current!=requested?1U:0U) || result!=(current!=requested?resultCode:2U) || memcmp(expectedObject,storage,sizeof(storage)))++errors;
  ++scenarios;
 }
 if(errors){printf("DISPLAY_MODE_FAIL errors=%u\n",errors);return 1;}printf("DISPLAY_MODE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
