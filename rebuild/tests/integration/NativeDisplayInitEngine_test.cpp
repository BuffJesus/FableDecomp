#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_definition_manager.h"
#include "fable_engine_init.h"
#include "display_init_engine_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" unsigned char __fastcall InitDisplayEngine(void*);
extern "C" unsigned Renderer=0,MainComponent=0,FadeRange=0;
#define StaticMap (*(unsigned*)&GStaticMapQuality)
#define Minimum GEnableMinimumStaticMapRebuild
#define Allow GAllowDataGeneration
#define BuildRetail GBuildRetailStaticMaps
#define Memory ((unsigned*)&g_FableEngineMemoryInit_0137549C)
extern "C" unsigned char Config[64]={0};
static unsigned char* arena;static unsigned char* init;static unsigned char cached[100];static bool live;static unsigned dtors;
static unsigned fill,flags,state,editor,world,mode,refs;static FILE* output;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static unsigned& iw(unsigned o){return *(unsigned*)(init+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};unsigned globals[]={Renderer,StaticMap,Minimum&255,Allow&255,BuildRetail&255,MainComponent,FadeRange};fwrite(h,4,4,output);fwrite(arena,1,4096,output);fwrite(live?init:cached,1,100,output);fwrite(globals,4,7,output);fwrite(Config,1,64,output);fwrite(Memory,4,10,output);fwrite(arena+0x26144,4,1,output);}
static void mutate(unsigned kind,unsigned label=0){
 if(mode==1&&kind==1)word(0x26144)=(unsigned)(arena+0xe40);
 else if(mode==2&&kind==2){word(0xf8)=0;word(0x2c)=(unsigned)(arena+0x480);}
 else if(mode==3&&kind==3)word(0x10)=(unsigned)(arena+0x880);
 else if(mode==4&&kind==20)iw(8)=(unsigned)(arena+0xe80);
 else if(mode==5&&kind==30&&label==0x20)iw(0x60)=0x4000;
 else if(mode==6&&kind==31&&label==0xff){StaticMap=0x12345678;Memory[0]=0xabcdef01;}
 else if(mode==7&&kind==4){iw(0x60)=0x2000;Allow=1;memset(Config+0x35,1,4);}
 else if(mode==8&&kind==40&&label==1)word(0x2c)=(unsigned)(arena+0x440);
 else if(mode==9&&kind==5){word(0x2c)=(unsigned)(arena+0x440);iw(0x5c)=0x76543210;}
 else if(mode==10){if(kind==40&&label==2)word(0x2c)=(unsigned)(arena+0x440);else if(kind==6)word(0x2c)=(unsigned)(arena+0x400);}
 else if(mode==11&&kind==7)word(0x2c)=(unsigned)(arena+0x440);
 else if(mode==12&&kind==40&&label==3)word(0x2c)=(unsigned)(arena+0x440);
 else if(mode==13&&kind==8)word(0x10)=(unsigned)(arena+0x880);
 else if(mode==14&&kind==9)word(0xb50)=0x3fc00000;
 else if(mode==15&&kind==10){FadeRange=0x7fc12345;word(0xdc)=0x55667788;}
 else if(mode==16&&kind==31&&label!=0xff)word(0x2c)=(unsigned)(arena+0x480);
}
extern "C" void* __fastcall ConstructInit(void* self){init=(unsigned char*)self;live=true;memset(init,fill^0x5a,100);for(unsigned i=0;i<5;++i)iw(0x20+i*4)=(unsigned)(arena+0xd80+i*16);event(1);mutate(1);return self;}
extern "C" void* __fastcall GetSystem(){event(2);mutate(2);return arena+0xf00;}
extern "C" void* __fastcall AllocateEngine(){event(3);mutate(3);return state?arena+0x400:0;}
#define GETTER(name,label) extern "C" void* __fastcall name(void* result){event(20+label);mutate(20+label);*(unsigned*)result=(unsigned)(arena+0xd00+label*16);return result;}
GETTER(Graphics,0) GETTER(Mesh,1) GETTER(Shader,2) GETTER(Cache,3) GETTER(WriteCache,4)
extern "C" void* __fastcall AssignWide(void* self,void*,void* other){unsigned label=(unsigned char*)self-init,value=*(unsigned*)other;if(label<0x20||label>0x30||label%4)exit(10);event(30,label,value);*(unsigned*)self=value;mutate(30,label);return self;}
extern "C" void __fastcall DestroyWide(void* self){unsigned label=(unsigned char*)self>=init+0x20&&(unsigned char*)self<init+0x34?(unsigned char*)self-init:0xff;event(31,label,*(unsigned*)self);*(unsigned*)self=0;mutate(31,label);if(label!=0xff&&++dtors==5){memcpy(cached,init,100);live=false;}}
extern "C" unsigned char __fastcall IsEditor(void* self){event(4,(unsigned)self);mutate(4);return (unsigned char)editor;}
extern "C" void* __fastcall ConstructChar(void* self,void*,const char* text,long length){if(length!=-1)exit(11);unsigned label=!strcmp(text,"Init Engine: Initialise")?1:!strcmp(text,"Init Engine: AddEngine")?2:!strcmp(text,"Init Engine: OnActivate")?3:!strcmp(text,"ENGINE")?4:0;if(!label)exit(12);event(32,label);*(unsigned*)self=label;return self;}
extern "C" void __fastcall DestroyChar(void* self){event(33,*(unsigned*)self);*(unsigned*)self=0;}
extern "C" void __fastcall Progress(void* text,bool debug,float fraction,bool force){unsigned label=*(unsigned*)text;if(debug||force||*(unsigned*)&fraction!=0xbf800000)exit(13);event(40,label);mutate(40,label);}
extern "C" unsigned char __fastcall InitialiseEngine(void* self,void*,void* arg){if(arg!=init)exit(14);event(5,(unsigned)self);mutate(5);return state==1?1:state==3?2:0;}
extern "C" void* __fastcall GetManager(){event(6);mutate(6);return arena+0xc00;}
extern "C" void __fastcall AddEngine(void* self,void*,void* engine){event(7,(unsigned)self,(unsigned)engine);mutate(7);}
extern "C" void __fastcall Activate(void* self){event(8,(unsigned)self);mutate(8);}
extern "C" bool __fastcall GetEngineDef(void* self,void*,void* text,long index,void** out){if(*(unsigned*)text!=4||index!=1)exit(15);event(9,(unsigned)self);*out=arena+0xb00;mutate(9);return true;}
template<> bool CDefinitionManager::GetDef<CEngineDef>(const CCharString& text,long index,CDefPointer<const CEngineDef>* out) const {
 return GetEngineDef((void*)this,0,(void*)&text,index,&out->Object);
}
extern "C" void __fastcall DeleteDef(void* self){event(10,(unsigned)self);mutate(10);}
int main(int argc,char** argv){
 if(StaticMap!=256||!Allow||Minimum||BuildRetail)return 17;
 for(unsigned k=0;k<10;++k)if(Memory[k]!=((k==1||k==5)?0xbf800000:0xffffffff))return 18;
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x40000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 word(0x8008)=(unsigned)&InitialiseEngine;word(0x80c4)=(unsigned)&Activate;word(0x8124)=(unsigned)&IsEditor;word(0x8204)=(unsigned)&DeleteDef;
 unsigned p[7],cases=0;while(fread(p,4,7,input)==7){fill=p[0];flags=p[1];state=p[2];editor=p[3];world=p[4];mode=p[5];refs=p[6];init=0;live=false;dtors=0;memset(cached,0,100);memset(arena,fill,4096);
  word(8)=(unsigned)(arena+0x10000);word(0x10)=(unsigned)(arena+0x800);word(0x14)=(unsigned)(arena+0xa40);word(0x18)=(unsigned)(arena+0xa80);word(0xf8)=world?(unsigned)(arena+0x600):0;word(0x400)=word(0x440)=(unsigned)(arena+0x8000);word(0x900)=(unsigned)(arena+0x8100);word(0xb00)=(unsigned)(arena+0x8200);word(0xb04)=refs;word(0xb50)=0x3f400000;word(0x26144)=(unsigned)(arena+0xe00);
  Renderer=(unsigned)(arena+0xa00);StaticMap=0xfedcba98;Minimum=flags&1;Allow=(flags>>1)&1;BuildRetail=(flags>>2)&1;MainComponent=(unsigned)(arena+0x900);FadeRange=0x55667788;memset(Config,fill^0xa5,64);Config[0x37]=(flags>>3)&1;Config[0x38]=(flags>>4)&1;Config[0x35]=(flags>>5)&1;Config[0x36]=(flags>>6)&1;for(unsigned i=0;i<10;++i)Memory[i]=0x10203040+i;
  unsigned result=InitDisplayEngine(arena);if(dtors!=5)return 16;event(99,result);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_DISPLAY_INIT_ENGINE_PASS cases=%u\n",cases);return 0;
}
