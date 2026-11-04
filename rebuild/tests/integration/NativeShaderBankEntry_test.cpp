#include "fable_shader_data_bank.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned sizeValue,chunkValue,positionValue,mode,strings;static unsigned* stream;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void requireAt(bool v,unsigned line){if(!v){printf("line %u mode %u size %u\n",line,mode,sizeValue);fflush(output);exit(21);}}
#define require(v) requireAt((v),__LINE__)
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){
 unsigned head[]={kind,a,b,c,d};fwrite(head,4,5,output);unsigned char copy[0x4000];memcpy(copy,arena,sizeof(copy));memset(copy,0,4);fwrite(copy,1,sizeof(copy),output);
 unsigned state[7]={0};if(stream)memcpy(state,stream,sizeof(state));state[0]=0;fwrite(state,4,7,output);
}
extern "C" void* __fastcall BaseCtor(void* self){require(self==arena);event(1);*(unsigned*)self=0x129a038;return self;}
extern "C" void* __fastcall CharDefault(void* self){require(self==arena+0x20);event(2,(unsigned)self);*(unsigned*)self=0;return self;}
extern "C" unsigned __fastcall EntrySize(void* self,void*,unsigned index){require(self==arena+0x800&&index==4);event(3,(unsigned)self,index);if(mode==1)word(4)=0xabcdef01;return sizeValue;}
extern "C" void* __cdecl FableEntryMalloc(size_t size){require(size==sizeValue&&size>0);event(4,size);return arena+0x1000;}
extern "C" void __fastcall EntryRead(void* self,void*,unsigned index,unsigned offset,void* dest,unsigned size){require(self==arena+0x800&&index==4&&offset==0&&size==sizeValue&&(size==0||dest==arena+0x1000));event(5,index,offset,size?(unsigned)dest:0,size);if(size)memcpy(dest,arena+0x3000,size);}
extern "C" void* __fastcall MemoryCtor(void* self,void*,const void* source,unsigned size){require(size==sizeValue&&(size==0||source==arena+0x1000));event(6,size?(unsigned)source:0,size);stream=(unsigned*)self;source=size?source:arena+0x3000;unsigned state[]={0x123675c,positionValue,size,(unsigned)source,0,chunkValue,(unsigned)source};memcpy(stream,state,sizeof(state));return self;}
extern "C" void __fastcall ReadOverflow(void* self,void*,void* dest,long size){require(self==stream&&size>=0&&size<=12);event(7,(unsigned)dest>=(unsigned)arena&&(unsigned)dest<(unsigned)(arena+0x4000)?(unsigned)dest:0,size);memcpy(dest,(void*)stream[3],size);stream[3]+=size;stream[1]+=size;stream[5]=mode==2?0:0x100;}
extern "C" void __fastcall ResizeCode(void* self,void*,unsigned count,const unsigned* value){require(self==arena+8&&count<=3&&*value==0);event(8,(unsigned)self,count,*value);unsigned start=count?(unsigned)(arena+0x1800):0;word(8)=start;word(12)=start+count*4;word(16)=start+count*4;if(count)memset((void*)start,0,count*4);}
extern "C" void* __fastcall ReadString(void* self,void*,unsigned* out){require(self==stream);unsigned token=0x40000000+strings;event(9,token);++strings;*out=token;stream[3]+=2;stream[1]+=2;stream[5]-=2;return out;}
extern "C" void* __fastcall CopyString(void* self,void*,const unsigned* source){require((unsigned)self>=(unsigned)(arena+0x2000)&&(unsigned)self<(unsigned)(arena+0x2040));event(10,(unsigned)self,*source);*(unsigned*)self=*source;return self;}
extern "C" void __fastcall GrowStrings(unsigned* self,void*,void* dest,const unsigned* value,void*,unsigned fill,bool atend){
 require(self==(void*)(arena+0x14)&&dest==(void*)word(0x18)&&fill==1&&atend);unsigned token=*value;event(11,(unsigned)self,token);unsigned start=self[0],count=start?(self[1]-start)/4:0;if(!start)start=(unsigned)(arena+0x2000);*(unsigned*)(start+count*4)=token;self[0]=start;self[1]=start+(count+1)*4;self[2]=start+(mode==1?count+1:8)*4;
}
extern "C" void __fastcall DestroyString(unsigned* self){event(12,*self);*self=0;if(mode==2)word(4)^=1;}
extern "C" void* __fastcall AssignString(void* self,void*,const unsigned* value){require(self==arena+0x20);event(13,(unsigned)self,*value);*(unsigned*)self=*value;return self;}
extern "C" void __fastcall MemoryDtor(void* self){require(self==stream);event(14);stream=0;}
extern "C" void __cdecl FableEntryFree(void* p){require(p==arena+0x1000);event(15,(unsigned)p);}
extern "C" void UnusedShaderEntryBoundary(){puts("unused boundary");exit(22);}
#include "shader_entry_aliases.h"
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[7],cases=0;
 while(fread(p,4,7,input)==7){
  if(fread(arena,1,0x4000,input)!=0x4000)return 5;chunkValue=p[2];positionValue=p[3];mode=p[4];sizeValue=p[6];stream=0;strings=0;
  CShaderBankEntry* entry=new(arena) CShaderBankEntry(mode,4,(CShaderDataBank*)(arena+0x800));require(entry==(void*)arena);event(16);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_SHADER_BANK_ENTRY_PASS cases=%u\n",cases);return 0;
}
