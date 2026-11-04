#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define abort() (fprintf(stderr,"fixture assertion line %d\n",__LINE__),exit(70))
#include "video_shadow_chain_aliases.h"
#include "video_shadow_chain_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall SetOptions(void*,void*,void*);
extern "C" void* VideoGlobal;extern "C" void* DisplayGlobal;extern "C" void* WeatherGlobal;extern "C" void* ColourGlobal;extern "C" void* WaterGlobal;extern "C" void* EngineGlobal;extern "C" void* ResourceGlobal;extern "C" void* PrimitiveGlobal;extern "C" void* LocalGlobal;extern "C" void* OcclusionGlobal;
extern "C" void* ShadowGlobal;extern "C" void* DisplacementGlobal;extern "C" void* GlowGlobal;extern "C" void* RadialGlobal;
extern "C" void* SurfaceGlobal;extern "C" const unsigned char FormatTable[1692];
static unsigned char* arena;
static unsigned char* nativeCode;
static unsigned char* inputOptions;
static unsigned p[7],events,displays,restores,weatherCalls,colourCalls,waterCalls;
static void* meshDefinition;static unsigned resourceFrames,poolResizes;
static FILE* output;
static unsigned& At(unsigned offset) {return *(unsigned*)(arena+offset);}
static void* Definition(unsigned char* engine,unsigned index) {if(index>3)abort();return *(void**)(engine+0x120+4*index);}
static void Record(unsigned tag,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0,unsigned e=0,unsigned f=0) {
    if(++events>160)abort();unsigned words[7]={tag,a,b,c,d,e,f};void* globals[15]={VideoGlobal,DisplayGlobal,WeatherGlobal,ColourGlobal,WaterGlobal,EngineGlobal,ResourceGlobal,PrimitiveGlobal,LocalGlobal,OcclusionGlobal,ShadowGlobal,DisplacementGlobal,GlowGlobal,RadialGlobal,SurfaceGlobal};
    fwrite(words,4,7,output);fwrite(globals,4,15,output);fwrite(arena,1,65536,output);
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
static bool __fastcall WaterQuality(void* self,void*,void* definition) {
    if(self!=WaterGlobal || definition!=Definition(arena,At(0xec)))abort();++waterCalls;
    if(p[3]==3)EngineGlobal=arena+0x200;
    Record(5,(unsigned)self,(unsigned)definition);return false;
}
extern "C" void __fastcall ResourceFrame(void* self,void*) {
    if(self!=arena+0x8000||++resourceFrames!=1)abort();
    unsigned char* bank=(unsigned char*)At(0x1e58);if(*(unsigned*)(bank+0x26c)||*(unsigned*)(bank+0x19c))abort();
    if(p[3]==4)At(0x8008)=0;
    if(p[3]==3)At(0x1e58)=(unsigned)(arena+0x7800);
    Record(17,(unsigned)self);
}
extern "C" bool __fastcall ResizePrealloc(void* self,void*,unsigned count) {
    void* definition=Definition((unsigned char*)EngineGlobal,At(0xe4));long memory=*(long*)((unsigned char*)definition+0x3c);
    unsigned expected=(unsigned)((long)((unsigned)memory<<10)/174776);
    if(self!=arena+0x804c+2*0x54||count!=expected||++poolResizes!=1)abort();
    *(unsigned*)((unsigned char*)self+0x3c)=count;
    if(p[3]==6)*(void**)((unsigned char*)At(0x1e58)+0x280)=arena+0x8800;
    Record(18,(unsigned)self,count);return (p[1]&1)!=0;
}
static void __fastcall ComponentRelease(void* self,void*) {
    if(self!=arena+0x1a00)abort();++At(0x1a04);Record(12);
}
static bool __fastcall ComponentRestore(void* self,void*) {
    if(self!=arena+0x1a00)abort();++At(0x1a08);++restores;bool ok=p[2]!=2;Record(13,ok);return ok;
}
static unsigned shadowCleanups,shadowInits;
extern "C" void __fastcall ShadowUninitialise(void* self,void*) {
    if(self!=ShadowGlobal || shadowCleanups!=shadowInits)abort();++shadowCleanups;
    unsigned size=*(unsigned*)((unsigned char*)self+0x338),scale=*(unsigned*)((unsigned char*)self+0x33c);
    unsigned quality=At(0xe8);if(p[2]==1)quality-=shadowCleanups-1;if(quality>3||size!=1000+quality||scale!=0x3f800000+quality)abort();
    ((unsigned char*)self)[0x2af]=0;Record(19,size,scale);
}
extern "C" bool __fastcall ShadowInitialise(void* self,void*) {
    if(self!=ShadowGlobal || shadowCleanups!=shadowInits+1 || ((unsigned char*)self)[0x2af])abort();++shadowInits;
    unsigned size=*(unsigned*)((unsigned char*)self+0x338);bool ok=!(p[2]==1 && size>1001);
    ((unsigned char*)self)[0x2af]=(unsigned char)ok;Record(14,size,ok);return ok;
}
static bool __fastcall Effect(void* self,void*,void* definition) {
    if(self!=DisplacementGlobal && self!=GlowGlobal && self!=RadialGlobal)abort();
    if(definition!=Definition(arena,At(0xec)))abort();Record(15,(unsigned)self,(unsigned)definition);return true;
}
static unsigned Target(unsigned address) {
    switch(address) {
    case 0x1436eb0:return (unsigned)&VideoGlobal;case 0x1436e1c:return (unsigned)&DisplayGlobal;case 0x1436e44:return (unsigned)&WeatherGlobal;case 0x1436e40:return (unsigned)&ColourGlobal;case 0x1436e54:return (unsigned)&WaterGlobal;case 0x1436ea4:return (unsigned)&EngineGlobal;case 0x1436e98:return (unsigned)&ResourceGlobal;case 0x1436e80:return (unsigned)&PrimitiveGlobal;case 0x1436e88:return (unsigned)&LocalGlobal;case 0x1436e58:return (unsigned)&OcclusionGlobal;
    case 0x9bff40:return (unsigned)&DisplayMode;case 0x9bff10:return (unsigned)&Interval;case 0xb50c60:return (unsigned)(nativeCode+848);case 0xb5a0b0:return (unsigned)(nativeCode+880);case 0xb3b4f0:return (unsigned)(nativeCode+1376);case 0x1436eac:return (unsigned)&SurfaceGlobal;
    case 0x9fd0c0:return (unsigned)(nativeCode+1520);case 0x9fd100:return (unsigned)(nativeCode+1568);case 0x9fc160:return (unsigned)(nativeCode+1600);
    case 0xa698f0:return (unsigned)(nativeCode+1616);case 0x9e3820:return (unsigned)(nativeCode+1760);case 0x9e3830:return (unsigned)(nativeCode+1776);
    case 0xb2ad80:return (unsigned)(nativeCode+1840);case 0xa6abe0:return (unsigned)(nativeCode+1856);case 0xa69730:return (unsigned)(nativeCode+1920);
    case 0xa698b0:return (unsigned)&ResourceFrame;case 0xa6a670:return (unsigned)&ResizePrealloc;case 0x129ba40:return (unsigned)FormatTable;case 0x129ba48:return (unsigned)(FormatTable+8);case 0xb32010:return (unsigned)(nativeCode+912);case 0xb93500:return (unsigned)(nativeCode+1056);case 0xb91ca0:return (unsigned)(nativeCode+1248);case 0xb59870:return (unsigned)(nativeCode+2032);case 0xb2f740:return (unsigned)(nativeCode+2368);case 0xb24500:return (unsigned)(nativeCode+448);
    case 0x1436e60:return (unsigned)&ShadowGlobal;case 0x1436e38:return (unsigned)&DisplacementGlobal;case 0x1436e34:return (unsigned)&GlowGlobal;case 0x1436e30:return (unsigned)&RadialGlobal;case 0xb4fc10:return (unsigned)(nativeCode+2384);case 0xb4b830:return (unsigned)&ShadowUninitialise;case 0xb4f8e0:return (unsigned)&ShadowInitialise;
    default:abort();return 0;
    }
}
int main(int argc,char** argv) {
    if(argc!=4||VideoGlobal||DisplayGlobal||WeatherGlobal||ColourGlobal||WaterGlobal||EngineGlobal||ResourceGlobal||PrimitiveGlobal||LocalGlobal||OcclusionGlobal||ShadowGlobal||DisplacementGlobal||GlowGlobal||RadialGlobal||SurfaceGlobal)return 2;
    FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;nativeCode=code;memcpy(code,retailCode,sizeof(retailCode));
    for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i) {
        unsigned value=Target(retailFixups[i][1]);if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;
    }
    static void* componentTable[13];componentTable[11]=(void*)&ComponentRelease;componentTable[12]=(void*)&ComponentRestore;
    static void* effectTable[14];effectTable[13]=(void*)&Effect;
    static void* waterTable[14];waterTable[13]=(void*)&WaterQuality;
    typedef bool (__fastcall *Fn)(void*,void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&SetOptions;unsigned cases=0;unsigned char table[1692];memcpy(table,FormatTable,1692);
    while(fread(p,4,7,in)==7) {
        if(fread(arena,1,65536,in)!=65536)return 6;events=displays=restores=weatherCalls=colourCalls=waterCalls=resourceFrames=poolResizes=0;inputOptions=arena+p[4];shadowCleanups=shadowInits=0;
        SurfaceGlobal=arena+0xa000;VideoGlobal=0;DisplayGlobal=arena+0x1100;WeatherGlobal=arena+0x3000;ColourGlobal=arena+0x4400;WaterGlobal=arena+0x1000;EngineGlobal=arena+(p[5]?0x200:0);ResourceGlobal=arena+0x1500;PrimitiveGlobal=arena+0x5000;LocalGlobal=p[6]?arena+0x1700:0;OcclusionGlobal=arena+0x1900;
        ShadowGlobal=arena+0xb000;DisplacementGlobal=arena+0x1d00;GlowGlobal=arena+0x1d40;RadialGlobal=arena+0x1d80;
        *(void***)(arena+0x1a00)=componentTable;*(void***)(arena+0x1d00)=effectTable;*(void***)(arena+0x1d40)=effectTable;*(void***)(arena+0x1d80)=effectTable;
        *(void***)(arena+0x1000)=waterTable;*(void***)(arena+0x1040)=waterTable;
        unsigned char expectedFlags[15];unsigned sceneCount=(At(0x533c)-At(0x5338))/4;
        bool willContinue=p[0]&&(p[1]%3==2||(p[1]/3)%3!=0||(p[1]/9)%3!=0);
        for(unsigned sc=0;sc<3;++sc)for(unsigned cell=0;cell<5;++cell) {
            unsigned offset=0x6a44+400*sc+72*cell;unsigned char flags=arena[offset];
            if(willContinue && sc<sceneCount && At(0x6800+32*sc) && (!cell || (At(0x660c+32*sc) && At(0x6804+32*sc+4*(cell-1)))))flags&=0xfe;
            expectedFlags[5*sc+cell]=flags;
        }
        unsigned resourceCount=At(0x8008),savedDelay=At(0x766c),bankFrame=At(0x7194),bankQueue=At(0x71d0),bankUnload=At(0x7268),compositionFrame=At(0xa52c),compositionQueue=At(0xa568),compositionUnload=At(0xa600);
        unsigned initialShadow=*(unsigned*)(inputOptions+0x28);bool result=fn(arena,0,inputOptions);
        if(resourceFrames!=(willContinue&&resourceCount>=3?1:0)||poolResizes!=resourceFrames||memcmp(table,FormatTable,1692))abort();
        if(willContinue){unsigned char* bank=(unsigned char*)At(0x1e58);if(*(unsigned*)(bank+0x26c)!=savedDelay||*(unsigned*)(bank+0x19c)!=savedDelay||At(0x7194)!=bankFrame+bankUnload||At(0x71d0)!=bankQueue+bankUnload||At(0xa52c)!=compositionFrame+compositionUnload||At(0xa568)!=compositionQueue+compositionUnload)abort();}
        for(sc=0;sc<3;++sc)for(unsigned cell=0;cell<5;++cell)if(arena[0x6a44+400*sc+72*cell]!=expectedFlags[5*sc+cell])abort();unsigned first=p[1]%3,second=(p[1]/3)%3,third=(p[1]/9)%3;
        bool continued=p[0]&&(first==2||second!=0||third!=0);
        unsigned displayCount=p[0]?(first==2?1:second?2:3):0;
        unsigned expectedRestores=continued?(p[2]==1&&initialShadow>1?initialShadow:1):0;
        if(shadowInits!=(p[2]==2?0:expectedRestores)||shadowCleanups!=shadowInits)abort();
        if(arena[0x1935]!=(continued?1:0x95))abort();
        if(result!=(p[0]&&first==2&&(p[2]==0||(p[2]==1&&initialShadow<=1))) || displays!=displayCount || restores!=expectedRestores || waterCalls!=(continued?1:0))abort();
        if(p[0] && arena[0xbd]!=1)abort();Record(11,result?1:0);++cases;
    }
    fclose(in);fclose(output);printf("NATIVE_VIDEO_SHADOW_CHAIN_PASS cases=%u\n",cases);return 0;
}
