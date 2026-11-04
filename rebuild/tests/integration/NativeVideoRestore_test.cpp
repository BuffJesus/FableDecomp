#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "video_restore_aliases.h"
#include "video_restore_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall SetOptions(void*,void*,void*);
extern "C" void* VideoGlobal;extern "C" void* DisplayGlobal;extern "C" void* WeatherGlobal;extern "C" void* ColourGlobal;extern "C" void* WaterGlobal;extern "C" void* EngineGlobal;extern "C" void* ResourceGlobal;extern "C" void* PrimitiveGlobal;extern "C" void* LocalGlobal;extern "C" void* OcclusionGlobal;
extern "C" void* ShadowGlobal;extern "C" void* DisplacementGlobal;extern "C" void* GlowGlobal;extern "C" void* RadialGlobal;
static unsigned char* arena;
static unsigned char* nativeCode;
static unsigned char* inputOptions;
static unsigned p[7],events,displays,restores,weatherCalls,colourCalls,waterCalls;
static void* meshDefinition;
static FILE* output;
static unsigned& At(unsigned offset) {return *(unsigned*)(arena+offset);}
static void* Definition(unsigned char* engine,unsigned index) {if(index>3)abort();return *(void**)(engine+0x120+4*index);}
static void Record(unsigned tag,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0,unsigned e=0,unsigned f=0) {
    if(++events>160)abort();unsigned words[7]={tag,a,b,c,d,e,f};void* globals[14]={VideoGlobal,DisplayGlobal,WeatherGlobal,ColourGlobal,WaterGlobal,EngineGlobal,ResourceGlobal,PrimitiveGlobal,LocalGlobal,OcclusionGlobal,ShadowGlobal,DisplacementGlobal,GlowGlobal,RadialGlobal};
    fwrite(words,4,7,output);fwrite(globals,4,14,output);fwrite(arena,1,8192,output);
}
extern "C" unsigned __fastcall DisplayMode(void* self,void*,const long* size,unsigned depth,unsigned multi,unsigned refresh) {
    if(self!=DisplayGlobal || ++displays>3 || depth!=*(unsigned*)(inputOptions+0x10))abort();
    long x=*(long*)inputOptions,y=*(long*)(inputOptions+4);if(displays==3){if(x>1024)x=1024;if(y>768)y=768;}
    if(size[0]!=x || size[1]!=y || multi!=(displays==1?*(unsigned*)(inputOptions+0x30):0) || refresh!=(displays==1?*(unsigned*)(inputOptions+0x14):0))abort();
    unsigned status=p[1];for(unsigned j=1;j<displays;++j)status/=3;status%=3;
    if(p[3]==1 && displays==1) {DisplayGlobal=arena+0x1200;*(long*)inputOptions+=17;*(long*)(inputOptions+4)-=9;*(unsigned*)(inputOptions+0x10)^=8;}
    if(p[3]==6 && displays==1)VideoGlobal=arena+0x1f00;
    Record(1,(unsigned)self,(unsigned)size[0],(unsigned)size[1],depth,multi,refresh);return status;
}
extern "C" void __fastcall Interval(void* self,void*,bool disable,long interval,bool apply) {
    if(self!=DisplayGlobal || disable!=(inputOptions[0x18]==0) || interval!=1 || !apply)abort();
    if(p[3]==2)At(0xec)=(At(0xec)+1)%4;
    Record(2,disable,interval,apply);
}
extern "C" void __fastcall WeatherQuality(void* self,void*,void* definition) {
    if(self!=WeatherGlobal || definition!=Definition(arena,At(0xec)))abort();++weatherCalls;
    if(p[3]==2)At(0xec)=(At(0xec)+1)%4;
    if(p[3]==7)WaterGlobal=arena+0x1040;
    Record(3,(unsigned)self,(unsigned)definition);
}
extern "C" void __fastcall ColourQuality(void* self,void*,void* definition) {
    if(self!=ColourGlobal || definition!=Definition(arena,At(0xec)))abort();++colourCalls;
    if(p[3]==2)At(0xec)=(At(0xec)+1)%4;
    Record(4,(unsigned)self,(unsigned)definition);
}
static bool __fastcall WaterQuality(void* self,void*,void* definition) {
    if(self!=WaterGlobal || definition!=Definition(arena,At(0xec)))abort();++waterCalls;
    if(p[3]==3)EngineGlobal=arena+0x200;
    Record(5,(unsigned)self,(unsigned)definition);return false;
}
extern "C" bool __fastcall ResizeMemory(void* self,void*,long amount) {
    void* definition=Definition((unsigned char*)EngineGlobal,At(0xe4));
    if(self!=ResourceGlobal || amount!=*(long*)((unsigned char*)definition+0x3c))abort();
    if(p[3]==3)EngineGlobal=arena;
    Record(6,(unsigned)self,amount);return false;
}
extern "C" void __fastcall DrawDistance(void* self,void*,unsigned quality,void* definition) {
    if(self!=PrimitiveGlobal || quality!=At(0xe0) || definition!=Definition((unsigned char*)EngineGlobal,quality))abort();meshDefinition=definition;
    if(p[3]==4) {LocalGlobal=(p[1]&1)?0:arena+0x1800;At(0xe0)=(quality+1)%4;}
    Record(7,(unsigned)self,quality,(unsigned)definition);
}
extern "C" void __fastcall LocalDistance(void* self,void*,void* definition) {
    if(self!=LocalGlobal || definition!=meshDefinition)abort();Record(8,(unsigned)self,(unsigned)definition);
}
extern "C" void __fastcall Scene(void* self,void*) {
    if(self!=OcclusionGlobal)abort();if(p[3]==5)arena[0xbd]=127;if(p[2]==1)At(0xe8)=2; ++restores;Record(9,(unsigned)self);
}
static void __fastcall ComponentRelease(void* self,void*) {
    if(self!=arena+0x1a00)abort();++At(0x1a04);Record(12);
}
static bool __fastcall ComponentRestore(void* self,void*) {
    if(self!=arena+0x1a00)abort();++At(0x1a08);bool ok=p[2]!=2;Record(13,ok);return ok;
}
extern "C" bool __fastcall Shadow(void* self,void*,long size,float scale) {
    if(self!=ShadowGlobal || size<1000 || size>1003 || *(unsigned*)&scale!=0x3f800000+size-1000)abort();
    bool ok=!(p[2]==1 && size>1001);Record(14,size,ok);return ok;
}
static bool __fastcall Effect(void* self,void*,void* definition) {
    if(self!=DisplacementGlobal && self!=GlowGlobal && self!=RadialGlobal)abort();
    if(definition!=Definition(arena,At(0xec)))abort();Record(15,(unsigned)self,(unsigned)definition);return true;
}
static unsigned Target(unsigned address) {
    switch(address) {
    case 0x1436eb0:return (unsigned)&VideoGlobal;case 0x1436e1c:return (unsigned)&DisplayGlobal;case 0x1436e44:return (unsigned)&WeatherGlobal;case 0x1436e40:return (unsigned)&ColourGlobal;case 0x1436e54:return (unsigned)&WaterGlobal;case 0x1436ea4:return (unsigned)&EngineGlobal;case 0x1436e98:return (unsigned)&ResourceGlobal;case 0x1436e80:return (unsigned)&PrimitiveGlobal;case 0x1436e88:return (unsigned)&LocalGlobal;case 0x1436e58:return (unsigned)&OcclusionGlobal;
    case 0x9bff40:return (unsigned)&DisplayMode;case 0x9bff10:return (unsigned)&Interval;case 0xb50c60:return (unsigned)&WeatherQuality;case 0xb5a0b0:return (unsigned)&ColourQuality;case 0xb3b4f0:return (unsigned)&ResizeMemory;case 0xb32010:return (unsigned)&DrawDistance;case 0xb59870:return (unsigned)&LocalDistance;case 0xb2f740:return (unsigned)&Scene;case 0xb24500:return (unsigned)(nativeCode+448);
    case 0x1436e60:return (unsigned)&ShadowGlobal;case 0x1436e38:return (unsigned)&DisplacementGlobal;case 0x1436e34:return (unsigned)&GlowGlobal;case 0x1436e30:return (unsigned)&RadialGlobal;case 0xb4fc10:return (unsigned)&Shadow;
    default:abort();return 0;
    }
}
int main(int argc,char** argv) {
    if(argc!=4||VideoGlobal||DisplayGlobal||WeatherGlobal||ColourGlobal||WaterGlobal||EngineGlobal||ResourceGlobal||PrimitiveGlobal||LocalGlobal||OcclusionGlobal||ShadowGlobal||DisplacementGlobal||GlowGlobal||RadialGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;nativeCode=code;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i) {
        unsigned value=Target(retailFixups[i][1]);if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;
    }
    static void* componentTable[13];componentTable[11]=(void*)&ComponentRelease;componentTable[12]=(void*)&ComponentRestore;
    static void* effectTable[14];effectTable[13]=(void*)&Effect;
    static void* waterTable[14];waterTable[13]=(void*)&WaterQuality;
    typedef bool (__fastcall *Fn)(void*,void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&SetOptions;unsigned cases=0;
    while(fread(p,4,7,in)==7) {
        if(fread(arena,1,8192,in)!=8192)return 6;events=displays=restores=weatherCalls=colourCalls=waterCalls=0;inputOptions=arena+p[4];
        VideoGlobal=0;DisplayGlobal=arena+0x1100;WeatherGlobal=arena+0x1300;ColourGlobal=arena+0x1400;WaterGlobal=arena+0x1000;EngineGlobal=arena+(p[5]?0x200:0);ResourceGlobal=arena+0x1500;PrimitiveGlobal=arena+0x1600;LocalGlobal=p[6]?arena+0x1700:0;OcclusionGlobal=arena+0x1900;
        ShadowGlobal=arena+0x1c00;DisplacementGlobal=arena+0x1d00;GlowGlobal=arena+0x1d40;RadialGlobal=arena+0x1d80;
        *(void***)(arena+0x1a00)=componentTable;*(void***)(arena+0x1d00)=effectTable;*(void***)(arena+0x1d40)=effectTable;*(void***)(arena+0x1d80)=effectTable;
        *(void***)(arena+0x1000)=waterTable;*(void***)(arena+0x1040)=waterTable;
        bool result=fn(arena,0,inputOptions);unsigned first=p[1]%3,second=(p[1]/3)%3,third=(p[1]/9)%3;
        bool continued=p[0]&&(first==2||second!=0||third!=0);
        unsigned displayCount=p[0]?(first==2?1:second?2:3):0;
        if(result!=(p[0]&&first==2&&p[2]==0) || displays!=displayCount || restores!=(continued?1:0) || weatherCalls!=(continued?1:0) || colourCalls!=(continued?1:0) || waterCalls!=(continued?1:0))abort();
        if(p[0] && arena[0xbd]!=1)abort();Record(11,result?1:0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_VIDEO_RESTORE_PASS cases=%u\n",cases);return 0;
}
