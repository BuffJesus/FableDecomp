#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[0x2c8/4];
static unsigned char original[0x2c8],expected[44];
static unsigned errors,calls,resultCode,overload;
static CDisplayManager* Object(){return reinterpret_cast<CDisplayManager*>(storage);}
CDisplayManager::ECreateDeviceSuccess CDisplayManager::ResetDevice(const CCreateDeviceParameters& parameters){
 if(this!=Object() || &parameters==&LastDeviceCreationParameters)++errors;++calls;
 const unsigned char* bytes=reinterpret_cast<const unsigned char*>(&parameters);
 if(memcmp(bytes,expected,29) || memcmp(bytes+32,expected+32,2) || memcmp(bytes+36,expected+36,8))++errors;
 if(!overload && memcmp(bytes+34,expected+34,2))++errors;
 Gamma=3.5f;return static_cast<ECreateDeviceSuccess>(resultCode);
}
int main(){unsigned scenarios=0;const long intervals[]={-1,0,1,2,2147483647L};
 for(unsigned oldImmediate=0;oldImmediate<2;++oldImmediate)for(unsigned immediate=0;immediate<2;++immediate)for(unsigned oldCount=0;oldCount<5;++oldCount)for(unsigned count=0;count<5;++count)for(unsigned oldMissed=0;oldMissed<2;++oldMissed)for(unsigned missed=0;missed<2;++missed)for(resultCode=0;resultCode<3;++resultCode)for(overload=0;overload<2;++overload){
  memset(storage,0xa7,sizeof(storage));CDisplayManager::CCreateDeviceParameters& p=Object()->LastDeviceCreationParameters;p.ExclusiveMode=true;p.PresentationInterval.PresentImmediate=oldImmediate!=0;p.PresentationInterval.PresentImmediateIfSyncMissed=oldMissed!=0;p.PresentationInterval.VSyncCount=intervals[oldCount];
  CDisplayManager::CPresentationIntervalDesc desc;memset(&desc,0x69,sizeof(desc));desc.PresentImmediate=immediate!=0;desc.PresentImmediateIfSyncMissed=missed!=0;desc.VSyncCount=intervals[count];
  memcpy(original,storage,sizeof(storage));memcpy(expected,&p,44);memcpy(expected+32,&desc,8);calls=0;
  if(overload)Object()->ChangePresentationInterval(desc.PresentImmediate,desc.VSyncCount,desc.PresentImmediateIfSyncMissed);else Object()->ChangePresentationInterval(desc);
  bool changed=oldImmediate!=immediate || oldCount!=count;if(changed){float gamma=3.5f;memcpy(original+0x2c4,&gamma,4);}
  if(calls!=(changed?1U:0U) || memcmp(original,storage,sizeof(storage)))++errors;++scenarios;
 }
 if(errors){printf("DISPLAY_PRESENTATION_FAIL errors=%u\n",errors);return 1;}printf("DISPLAY_PRESENTATION_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
