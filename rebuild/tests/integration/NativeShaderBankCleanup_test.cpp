#include "fable_shader_data_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned countValue,mode,indexValue,deleteCount;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void requireAt(bool v,unsigned line){if(!v){printf("line %u\n",line);fflush(output);exit(21);}}
#define require(v) requireAt((v),__LINE__)
static void event(unsigned kind,unsigned a=0,unsigned b=0){unsigned header[]={kind,a,b};fwrite(header,4,3,output);unsigned char copy[0x1000];memcpy(copy,arena,sizeof(copy));memset(copy+0x804,0,4);fwrite(copy,1,sizeof(copy),output);}
extern "C" void* __fastcall DeleteEntry(void* self,void*,unsigned flags){
 require((unsigned)self>=(unsigned)(arena+0x400)&&(unsigned)self<(unsigned)(arena+0x600)&&flags==1);event(1,(unsigned)self,1);++deleteCount;
 if(mode==1){word(0x110)=(unsigned)(arena+0x280);word(0x114)=(unsigned)(arena+0x280+countValue*4);}
 else if(mode==2)word(0x114)=word(0x110)+4;
 else if(mode==3&&deleteCount==1)word(0x114)=word(0x110)+(countValue+2)*4;
 return self;
}
void CBankFile::Close(){require(this==(void*)arena&&word(0x114)==word(0x110));event(2,(unsigned)this);word(4)=0x12345678;}
extern "C" void __fastcall ReleaseState(void* self){require(self==arena+0x880);event(3,(unsigned)self);if(mode){word(0x800)=0x89abcdef;word(0x808)=(unsigned)(arena+0x8c0);}}
void operator delete(void* value){require(value==arena+0x800);event(4,(unsigned)value);}
extern "C" void* __fastcall CopySymbol(void* self,void*,const unsigned* source){require(self==arena+0x700&&source==(void*)(arena+0x600+indexValue*4));event(6,(unsigned)source,*source);*(unsigned*)self=*source;if(mode){word(8)=(unsigned)(arena+0x680);*(unsigned*)source=0xabcdef98;}return self;}
extern "C" void* __fastcall UnknownSymbol(void* self,void*,const char* text,long length){require(self==arena+0x700&&length==-1&&strcmp(text,"(Unknown)")==0);event(5,0x55443322);*(unsigned*)self=0x55443322;if(mode)word(8)=(unsigned)(arena+0x680);return self;}
#pragma comment(linker,"/alternatename:??0CCharString@@QAE@PBDJ@Z=@UnknownSymbol@16")
#pragma comment(linker,"/alternatename:??0CCharString@@QAE@ABV0@@Z=@CopySymbol@12")
// ABI-only invocations expose private/protected providers without supplying bodies.
extern "C" void __fastcall InvokeCloseShaders(void*);
extern "C" void __fastcall InvokeClose(void*);
extern "C" void __fastcall InvokeRemove(void*,void*,unsigned);
extern "C" unsigned __fastcall InvokeVersion(void*);
extern "C" void __fastcall InvokePrepare(void*,void*,unsigned,void*,unsigned,void*,void*,void*,void*);
extern "C" void* __fastcall InvokeSymbol(void*,void*,void*,unsigned);
#pragma comment(linker,"/alternatename:@InvokeCloseShaders@4=?CloseShaders@CShaderDataBank@@AAEXXZ")
#pragma comment(linker,"/alternatename:@InvokeClose@4=?Close@CShaderDataBank@@UAEXXZ")
#pragma comment(linker,"/alternatename:@InvokeRemove@12=?NotifyEntryRemoved@CShaderDataBank@@MAEXK@Z")
#pragma comment(linker,"/alternatename:@InvokeVersion@4=?GetDataFormatVersion@CShaderDataBank@@MAEKXZ")
#pragma comment(linker,"/alternatename:@InvokePrepare@36=?PrepareDataForInclusion@CShaderDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z")
#pragma comment(linker,"/alternatename:@InvokeSymbol@16=?GetEntrySymbol@CBankFile@@QBE?AVCCharString@@K@Z")
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x9000)=(unsigned)&DeleteEntry;
 unsigned p[7],cases=0;
 while(fread(p,4,7,input)==7){
  if(fread(arena,1,0x1000,input)!=0x1000)return 5;countValue=p[1];mode=p[3];indexValue=p[4];deleteCount=0;word(0x804)=(unsigned)&ReleaseState;
  unsigned result=0;
  if(p[0]==0)InvokeCloseShaders(arena);
  else if(p[0]==1)InvokeClose(arena);
  else if(p[0]==2)InvokeRemove(arena,0,indexValue);
  else if(p[0]==3){result=InvokeVersion(arena);require(result==107);}
  else if(p[0]==4)InvokePrepare(arena,0,123,arena+0x900,456,arena+0x880,p[6]?0:arena+0x800,arena+0x920,arena+0x940);
  else {result=(unsigned)InvokeSymbol(arena,0,arena+0x700,indexValue);require(result==(unsigned)(arena+0x700));}
  event(7,p[0],result);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_SHADER_BANK_CLEANUP_PASS cases=%u\n",cases);return 0;
}
