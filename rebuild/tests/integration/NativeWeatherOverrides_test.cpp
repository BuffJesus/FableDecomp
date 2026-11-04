#include <stdio.h>
#include <string.h>
#include <float.h>
#include "weather_overrides_aliases.h"
#include "weather_overrides_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
// Pass the float argument bits through the integer register/stack ABI unchanged.
extern "C" void __fastcall EngineRain(void*,void*,unsigned);
extern "C" void __fastcall EngineSnow(void*,void*,unsigned);
extern "C" void __fastcall OwnerRain(void*,void*,unsigned);
extern "C" void __fastcall OwnerSnow(void*,void*,unsigned);
extern "C" void* WeatherGlobal;
static unsigned char* code;
static const unsigned zero=0;
static unsigned resolve(unsigned target){
    if(target==0x1436e44)return(unsigned)&WeatherGlobal;
    if(target==0x122dedc)return(unsigned)&zero;
    if(target==0xb50ee0)return(unsigned)(code+512);
    if(target==0xb50f40)return(unsigned)(code+768);
    throw 1;
}
int main(int argc,char** argv){
    if(argc!=4||WeatherGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");FILE* out=fopen(argv[2],"wb");if(!in||!out)return 3;
    unsigned char* arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 6;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){
        unsigned offset=retailFixups[i][0],target=resolve(retailFixups[i][1]);
        if(retailFixups[i][2])target-=(unsigned)(code+offset+4);
        *(unsigned*)(code+offset)=target;
    }
    typedef void (__fastcall *Setter)(void*,void*,unsigned);
    Setter nativeFunctions[]={(Setter)code,(Setter)(code+256),(Setter)(code+512),(Setter)(code+768)};
    Setter compiledFunctions[]={EngineRain,EngineSnow,OwnerRain,OwnerSnow};
    unsigned p[4],cases=0;
    while(fread(p,4,4,in)==4){
        if(fread(arena,1,8192,in)!=8192)return 5;
        WeatherGlobal=arena+p[1];_clearfp();_control87(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);
        Setter f=argv[3][0]=='r'?nativeFunctions[p[0]]:compiledFunctions[p[0]];
        for(unsigned n=0;n<2;++n){
            f(p[0]<2?(void*)0x1badb002:WeatherGlobal,0,p[2+n]);
            unsigned status=_statusfp();fwrite(&status,4,1,out);fwrite(&WeatherGlobal,4,1,out);fwrite(arena,1,8192,out);
        }
        ++cases;
    }
    fclose(in);fclose(out);printf("NATIVE_WEATHER_OVERRIDES_PASS cases=%u\n",cases);return 0;
}
