#include <new>
#include "fable_game_text_bank.h"
#include <stdio.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
namespace _STL {
template<> void vector<CBankFile::CRuntimeData>::resize(size_t,CBankFile::CRuntimeData const&);
template<> void vector<CBankFile::CRuntimeData>::_M_fill_insert(CBankFile::CRuntimeData*,size_t,CBankFile::CRuntimeData const&);
template<> void vector<CBankFile::CRuntimeData>::_M_insert_overflow(CBankFile::CRuntimeData*,CBankFile::CRuntimeData const&,const __false_type&,size_t,bool);
}
static unsigned char* arena;static FILE* output;static unsigned mode,heap;static bool mutated;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);fwrite(arena,1,0x4000,output);}
extern "C" void* __cdecl BankRuntimeMalloc(size_t size){event(1,size);void* result=arena+0x1000+heap++*0x100;if(mode==1)word(0x900)^=0x101;else if(mode==2)word(8)=(unsigned)(arena+0x3040);return result;}
extern "C" void* __cdecl BankRuntimeMemmove(void* dest,const void* source,size_t size){event(2,(unsigned)dest,(unsigned)source,size);if(mode==1&&size)*(unsigned*)source^=0x100;if(mode==2&&!mutated){word(4)+=4;mutated=true;}return memmove(dest,source,size);}
extern "C" void __cdecl BankRuntimeFree(void* pointer){event(3,(unsigned)pointer);if(mode==3){word(0)=(unsigned)(arena+0x3000);word(4)=(unsigned)(arena+0x3004);word(8)=(unsigned)(arena+0x3040);}}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[8],cases=0;while(fread(p,4,8,input)==8){if(fread(arena,1,0x4000,input)!=0x4000)return 5;mode=p[7];heap=0;mutated=false;
 _STL::vector<CBankFile::CRuntimeData>* v=(_STL::vector<CBankFile::CRuntimeData>*)arena;CBankFile::CRuntimeData* start=(CBankFile::CRuntimeData*)word(0);CBankFile::CRuntimeData& value=*(CBankFile::CRuntimeData*)(arena+(p[5]?0x200:0x900));_STL::__false_type& tag=*(_STL::__false_type*)(arena+0x940);unsigned result=0;
 switch(p[0]){
 case 0:v->resize(p[3],value);break;
 case 1:v->_M_fill_insert(start+p[3],p[4],value);break;
 case 3:v->_M_insert_overflow(start+p[3],value,tag,p[4],p[6]!=0);break;
 }event(4,p[0],result);++cases;}
 fclose(input);fclose(output);printf("NATIVE_BANK_RUNTIME_VECTOR_PASS cases=%u\n",cases);return 0;
}