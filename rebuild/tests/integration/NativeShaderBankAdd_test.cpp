#include "fable_shader_data_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned countValue,mode,indexValue,typeValue,failValue,heap;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void requireAt(bool v,unsigned line){if(!v){printf("line %u mode %u index %u\n",line,mode,indexValue);fflush(output);exit(21);}}
#define require(v) requireAt((v),__LINE__)
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){unsigned head[]={kind,a,b,c,d};fwrite(head,4,5,output);fwrite(arena,1,0x4000,output);}
extern "C" void* __cdecl FableShaderMalloc(size_t size){
 require(size>0&&size<=128);event(1,size);void* result=arena+0x1000+heap++*0x100;
 if(mode==1)word(0x118)=(unsigned)(arena+0x3040);
 else if(mode==2){word(0x200)^=0x100;word(0x900)^=0x101;}
 return result;
}
extern "C" void* __cdecl FableShaderMemmove(void* dest,const void* source,size_t size){require(size<=128);event(2,(unsigned)dest,(unsigned)source,size);return memmove(dest,source,size);}
extern "C" void __cdecl FableShaderFree(void* pointer){event(3,(unsigned)pointer);if(mode==3){word(0x110)=(unsigned)(arena+0x3000);word(0x114)=(unsigned)(arena+0x3004);word(0x118)=(unsigned)(arena+0x3040);}}
void* operator new(size_t size){require(size==0x24);event(4,size);return failValue?0:arena+0x600;}
extern "C" void* __fastcall ConstructEntry(void* self,void*,unsigned type,unsigned index,void* bank){
 require(self==arena+0x600&&type==typeValue&&index==indexValue&&bank==arena);event(5,(unsigned)self,type,index,(unsigned)bank);memset(self,0x6a,0x24);*(unsigned*)((char*)self+4)=type;
 if(mode==1){word(0x110)=(unsigned)(arena+0x3000);word(0x114)=(unsigned)(arena+0x3000+(index+1)*4);word(0x118)=(unsigned)(arena+0x3040);}
 return self;
}
#pragma comment(linker,"/alternatename:??0CShaderBankEntry@@QAE@KKPAVCShaderDataBank@@@Z=@ConstructEntry@20")
void CBankFile::NotifyEntryAdded(unsigned long type,unsigned long index,const CArray<char>& data){require(this==(void*)arena&&type==typeValue&&index==indexValue&&&data==(void*)(arena+0x700));event(6,(unsigned)this,type,index,(unsigned)&data);word(4)=0x98760000|index;}
extern "C" void __fastcall InvokeAdd(void*,void*,unsigned,unsigned,void*);
#pragma comment(linker,"/alternatename:@InvokeAdd@20=?NotifyEntryAdded@CShaderDataBank@@MAEXKKABV?$CArray@D@@@Z")
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[10],cases=0;
 while(fread(p,4,10,input)==10){
  if(fread(arena,1,0x4000,input)!=0x4000)return 5;countValue=p[1];indexValue=p[3];typeValue=p[4];mode=p[5];failValue=p[6];heap=0;
  if(p[0]==0)InvokeAdd(arena,0,typeValue,indexValue,arena+0x700);
  else {
   typedef _STL::vector<CShaderBankEntry*> Vector;Vector* vector=(Vector*)(arena+0x110);
   CShaderBankEntry* const* value=(CShaderBankEntry* const*)(arena+(p[9]?0x200:0x900));
   vector->_M_insert_overflow(vector->begin()+indexValue,*value,_STL::__true_type(),p[7],p[8]!=0);
  }
  event(7,p[0]);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_SHADER_BANK_ADD_PASS cases=%u\n",cases);return 0;
}
