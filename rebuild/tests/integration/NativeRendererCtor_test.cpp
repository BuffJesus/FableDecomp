#include "fable_display_viewport.h"
#include "fable_render2d_shader.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;
static FILE* output;
static unsigned flagsValue,existsValue,ownership,oldValue,mode;
static unsigned& word(unsigned off){return *(unsigned*)(arena+off);}
static void requireAt(bool v,unsigned line){if(!v){printf("require line %u mode %u\n",line,mode);fflush(output);exit(21);}}
#define require(v) requireAt((v),__LINE__)
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){
 unsigned head[]={kind,a,b,c};fwrite(head,4,4,output);unsigned char copy[0x6000];memcpy(copy,arena,sizeof(copy));
 memset(copy,0,4);memset(copy+0x3d20,0,4);memset(copy+0x3e3c,0,4);memset(copy+0x3e50,0,4);memset(copy+0x5404,0,4);
 fwrite(copy,1,sizeof(copy),output);fwrite((void*)0x13ca79c,1,0x38,output);fwrite((void*)0x13bc470,1,0x3dc,output);
}
extern "C" void* __fastcall CoreCtor(void* self,void*,void* display,unsigned flags){
 require(self==arena && display==arena+0x4000 && flags==flagsValue);event(1,(unsigned)self,(unsigned)display,flags);memset(self,0x5a,0x3d20);return self;
}
extern "C" void* __fastcall BankCtor(void* self){require(self==arena+0x3d20);event(2,(unsigned)self);memset(self,0x6b,0x128);*(unsigned*)self=(unsigned)(arena+0x9000);return self;}
extern "C" void* __fastcall VertexCtor(void* self){require(self==arena+0x3e50);event(3,(unsigned)self);*(unsigned*)self=0x129b3d0;*(unsigned*)((char*)self+4)=0;return self;}
extern "C" void* __fastcall WideCtor(void* self,void*,const wchar_t* text){require(wcscmp(text,L"pc\\")==0);event(4,0x10);*(unsigned*)self=0x10;return self;}
extern "C" void* __fastcall WideAdd(void* out,const unsigned* left,const unsigned* right){require(left==(void*)(arena+0x5500)&&*right==0x10);event(5,*left,*right);*(unsigned*)out=0x20;return out;}
extern "C" void* __fastcall WideAddLiteral(void* out,const unsigned* left,const wchar_t* text){require(*left==0x20&&wcscmp(text,L"shaders.big")==0);event(6,0x20);*(unsigned*)out=0x30;return out;}
extern "C" void __fastcall OpenRetail(void* self,void*,const unsigned* path,bool priority){require(self==(void*)0x13ca79c&&*path==0x30&&!priority);event(7,(unsigned)self,0x30);if(mode)word(0x5500)=0x77777777;}
extern "C" void __fastcall WideDtor(unsigned* self){event(8,*self);*self=0;}
extern "C" void* __fastcall CharCtor(void* self,void*,const char* text,long length){unsigned token=strcmp(text,"SHADERS_BBBLIB")==0?0x40:0x50;if(token==0x50)require(strcmp(text,"VSHADER_BBBLIB_2D")==0);require(length==-1);event(9,token);*(unsigned*)self=token;if(token==0x50&&mode==2)word(0x3e48)=(unsigned)(arena+0x5300);return self;}
extern "C" bool __fastcall BankExists(void* self,void*,const unsigned* name){require(self==(void*)0x13ca79c&&*name==0x40);event(10,(unsigned)self,0x40);return existsValue!=0;}
extern "C" bool __fastcall OpenReadOnly(void* self,void*,const unsigned* name,unsigned flags){require(self==arena+0x3d20&&*name==0x40&&flags==0x48);event(11,(unsigned)self,0x40,0x48);return false;}
extern "C" void* __fastcall CreateResource(void* self,void*,unsigned* out){require(self==(void*)0x13bc470);event(12,(unsigned)self);out[0]=(unsigned)(arena+0x5200);out[1]=ownership?(unsigned)(arena+0x5400):0;return out;}
extern "C" void* __fastcall AssignResource(unsigned* self,void*,const unsigned* other){require(self==(void*)(arena+0x3e48));unsigned data=other[0],info=other[1];event(13,(unsigned)self,data,info);self[0]=data;self[1]=info;if(info&&mode!=1)++*(unsigned*)info;return self;}
extern "C" void __fastcall ReleaseResource(void* self){require(self==arena+0x5200);event(14,(unsigned)self);if(mode)word(0x3e48)=(unsigned)(arena+0x5300);}
void operator delete(void* value){require(value==arena+0x5400);event(15,(unsigned)value);}
extern "C" void __fastcall OpenShaderBank(void* self,void*,void* bank){require((self==arena+0x5200||self==arena+0x5300)&&bank==arena+0x3d20);event(16,(unsigned)self,(unsigned)bank);if(mode==1)word(0x3e48)=(unsigned)(arena+0x5300);}
extern "C" void __fastcall SetDeclaration(void* self,void*,const void* decl){require((self==arena+0x5200||self==arena+0x5300)&&decl==Fable2DShaderDeclaration_0129D708);event(17,(unsigned)self,0x129d708);if(mode==2)word(0x3e48)=(unsigned)(arena+0x5200);}
extern "C" void __fastcall SetFormat(void* self,void*,unsigned format){require((self==arena+0x5200||self==arena+0x5300)&&format==0x1c4);event(18,(unsigned)self,format);}
extern "C" void* __fastcall LookupVertex(void* self,void*,unsigned* out,const unsigned* name){require((self==arena+0x5200||self==arena+0x5300)&&*name==0x50);event(19,(unsigned)self,0x50);out[0]=0x129b3d0;out[1]=(unsigned)(arena+0x5040);word(0x3e54)=oldValue?(unsigned)(arena+0x5000):0;return out;}
extern "C" void* __fastcall DeleteVertex(void* self,void*,unsigned flags){require((self==arena+0x5000||self==arena+0x5040)&&flags==1);event(20,(unsigned)self,1);if(self==arena+0x5000&&mode==2)word(0x5050)^=0x100;return self;}
extern "C" void __fastcall CharDtor(unsigned* self){event(21,*self);*self=0;}
extern "C" void UnusedRendererCtorBoundary(){puts("unused boundary reached");exit(22);}
#include "renderer_ctor_aliases.h"
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 if(VirtualAlloc((void*)0x1390000,0x40000,0x3000,4)!=(void*)0x1390000)return 5;
 word(0x9004)=(unsigned)&OpenReadOnly;word(0x9200)=(unsigned)&DeleteVertex;
 unsigned params[7],cases=0;
 while(fread(params,4,7,input)==7){
  if(fread(arena,1,0x6000,input)!=0x6000||fread((void*)0x13ca79c,1,0x38,input)!=0x38||fread((void*)0x13bc470,1,0x3dc,input)!=0x3dc)return 6;
  flagsValue=params[0];existsValue=params[2];ownership=params[3];oldValue=params[4];mode=params[5];word(0x5404)=(unsigned)&ReleaseResource;
  CRenderManager2D* manager=new(arena) CRenderManager2D(*(CDisplayManager*)(arena+0x4000),*(CWideString*)(arena+0x5500),flagsValue);
  require(manager==(void*)arena&&arena[0x3ea0]==(flagsValue&1));event(22);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_RENDERER_CTOR_PASS cases=%u\n",cases);return 0;
}
