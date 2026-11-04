#include "fable_game_text_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned indexValue,lengthValue,mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void requireAt(bool v,unsigned line){if(!v){printf("line %u\n",line);exit(21);}}
#define require(v) requireAt((v),__LINE__)
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);fwrite(arena,1,0x1000,output);}
unsigned long CPackedUIntArray::operator[](unsigned long index) const {
 require((this==(void*)(arena+0x38)||this==(void*)(arena+0x48))&&index==indexValue-1);event(1,(unsigned)this,index);
 if(this==(void*)(arena+0x48)){
  if(mode==1){word(0x3c)=1-word(0x3c);word(0x20)=(unsigned)(arena+0x300);word(0x7c)=(unsigned)(arena+0x540);}
  return 0x76543210;
 }
 unsigned result=word(0x900);if(mode==2)word(0x7c)=(unsigned)(arena+0x580);return result;
}
extern "C" void __fastcall SeekBoundary(void* self,void*,long position){require(self==arena+0x500||self==arena+0x540||self==arena+0x580);event(2,(unsigned)self,position);*(unsigned*)((char*)self+4)=position;if(mode){word(0x7c)=(unsigned)(arena+0x540);word(0x900)^=0x80000000;}}
extern "C" void __fastcall ReadBoundary(void* self,void*,void* data,long length,void* process){require((self==arena+0x500||self==arena+0x540||self==arena+0x580)&&data==arena+0x700&&(unsigned)length==lengthValue&&!process);event(3,(unsigned)self,(unsigned)data,length);word(0x700)=0x12340000|mode;}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8014)=(unsigned)&SeekBoundary;word(0x800c)=(unsigned)&ReadBoundary;
 unsigned p[4],cases=0;
 while(fread(p,4,4,input)==4){if(fread(arena,1,0x1000,input)!=0x1000)return 5;indexValue=p[0];lengthValue=p[2];mode=p[3];((CBankFile*)arena)->ReadEntryData(indexValue,p[1],arena+0x700,lengthValue);event(4);++cases;}
 fclose(input);fclose(output);printf("NATIVE_BANK_READ_PASS cases=%u\n",cases);return 0;
}
