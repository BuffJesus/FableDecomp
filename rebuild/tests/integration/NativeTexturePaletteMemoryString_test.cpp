#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "palette_memory_string_aliases.h"
#include "palette_memory_string_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Load(void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events,names,found;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>100)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){record(1,self,n);if(n!=1&&n!=4)exit(21);memcpy(out,(void*)at(0x20c),n);at(0x20c)+=n;at(0x204)+=n;at(0x214)=32;if(mode==6)at(0x10)=0x76543210;}
// The boundary initializes the companion index of the native default temporary.
extern "C" void __fastcall Default(void* self){((unsigned*)self)[-1]=0x12345678;*(unsigned*)self=0x1111;record(2,0,0);if(mode==5)arena[0x14]=0x80;}
extern "C" void __fastcall Copy(void* self,void*,const void* other){*(unsigned*)self=*(unsigned*)other;record(8,0,*(unsigned*)other);}
extern "C" void* __cdecl Allocate(unsigned n){if(n>128)exit(22);record(9,0,n);return arena+0x800;}
extern "C" void __cdecl Free(void* p){record(10,p,0);}
static void* stable(void* p){return (unsigned char*)p>=arena&&(unsigned char*)p<arena+4096?p:0;}
extern "C" void* __fastcall ReadName(void*,void*,void*);
typedef void* (__fastcall *ReadNameFn)(void*,void*,void*);
static ReadNameFn readName;
extern "C" void* __fastcall DispatchReadName(void* self,void* ignored,void* result){return readName(self,ignored,result);}
extern "C" unsigned __fastcall Position(void* self){record(11,self,((unsigned*)self)[1]);return ((unsigned*)self)[1];}
extern "C" void* __fastcall Construct(void* self,void*,const char* text,long n){if(n!=-1||names>=4||text<(char*)arena+0x305||text>(char*)arena+0x320)exit(23);unsigned size=strlen(text);if(size>4)exit(24);unsigned off=0xa00+names++*32;at(off)=1;at(off+4)=size;memcpy(arena+off+16,text,size+1);*(unsigned*)self=(unsigned)(arena+off);record(3,(void*)text,size);if(mode==2)at(4)=(unsigned)(arena+0x600);return self;}
extern "C" void* __fastcall Assign(void* self,void*,const void* other){record(4,stable(self),*(unsigned*)other);*(unsigned*)self=*(unsigned*)other;return self;}
extern "C" void __fastcall Destroy(void* self){record(5,0,*(unsigned*)self);if(mode==3){at(0)=(unsigned)(arena+0x440);at(0x10)=0x87654321;}}
extern "C" unsigned __fastcall Find(void* self,void*,const void* name){record(6,self,*(unsigned*)name);if(mode==4)at(4)=(unsigned)(arena+0x600);return found;}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0x993ca0:v=(unsigned)Overflow;break;case 0x99efb0:v=(unsigned)Assign;break;case 0x99eae0:v=(unsigned)Destroy;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0xbf97a0:v=(unsigned)(code+512);break;case 0xbf9420:v=(unsigned)(code+640);break;case 0xbf95d0:v=(unsigned)(code+736);break;case 0xbf8e50:v=(unsigned)(code+1024);break;case 0xbf8e90:v=(unsigned)(code+1088);break;case 0xbf8e20:v=(unsigned)(code+1152);break;case 0xbf8ff0:v=(unsigned)(code+1216);break;case 0xbf94d0:v=(unsigned)(code+1280);break;case 0xbf8f30:v=(unsigned)(code+1536);break;case 0xbf8f70:v=(unsigned)(code+1600);break;case 0x99e4b0:v=(unsigned)Default;break;case 0x99ec30:v=(unsigned)Copy;break;case 0xbfea0e:v=(unsigned)Allocate;break;case 0xbfea14:v=(unsigned)Free;break;case 0x99ebf0:v=(unsigned)Construct;break;default:return 8;}
 *(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,void*);Fn fn=argv[3][0]=='r'?(Fn)code:Load;
 readName=argv[3][0]=='r'?(ReadNameFn)(code+1792):ReadName;
 unsigned p[2],cases=0;while(fread(p,4,2,in)==2){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];found=p[1];events=names=0;at(0x200)=(unsigned)(arena+0x700);at(0x708)=(unsigned)Position;at(0x718)=(unsigned)DispatchReadName;
 fn(arena,0,arena+0x200);record(7,0,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_TEXTURE_PALETTE_MEMORY_STRING_PASS cases=%u\n",cases);return 0;
}
