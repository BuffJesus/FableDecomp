#include <new>
#include "fable_stlport_vector.h"
#include <stdio.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
namespace _STL {
template<> void vector<unsigned long>::resize(size_t,const unsigned long&);
template<> unsigned long* vector<unsigned long>::erase(unsigned long*,unsigned long*);
template<> void vector<unsigned long>::_M_fill_insert(unsigned long*,size_t,const unsigned long&);
template<> void vector<unsigned long>::_M_insert_overflow(unsigned long*,const unsigned long&,const __true_type&,size_t,bool);
template<> unsigned long* FABLE_FASTCALL __uninitialized_copy(const unsigned long*,const unsigned long*,unsigned long*,const __true_type&);
}
static unsigned char* arena;static FILE* output;static unsigned mode,heap;static bool mutated;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);fwrite(arena,1,0x4000,output);}
extern "C" void* __cdecl FableUlongMalloc(size_t size){event(1,size);void* result=arena+0x1000+heap++*0x100;if(mode==1)word(0x900)^=0x101;else if(mode==2)word(8)=(unsigned)(arena+0x3040);return result;}
extern "C" void* __cdecl FableUlongMemmove(void* dest,const void* source,size_t size){event(2,(unsigned)dest,(unsigned)source,size);if(mode==1&&size)*(unsigned*)source^=0x100;if(mode==2&&!mutated){word(4)+=4;mutated=true;}return memmove(dest,source,size);}
extern "C" void __cdecl FableUlongFree(void* pointer){event(3,(unsigned)pointer);if(mode==3){word(0)=(unsigned)(arena+0x3000);word(4)=(unsigned)(arena+0x3004);word(8)=(unsigned)(arena+0x3040);}}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[8],cases=0;while(fread(p,4,8,input)==8){if(fread(arena,1,0x4000,input)!=0x4000)return 5;mode=p[7];heap=0;mutated=false;
 _STL::vector<unsigned long>* v=(_STL::vector<unsigned long>*)arena;unsigned long* start=(unsigned long*)word(0);unsigned long& value=*(unsigned long*)(arena+(p[5]?0x200:0x900));_STL::__true_type& tag=*(_STL::__true_type*)(arena+0x940);unsigned result=0;
 switch(p[0]){
 case 0:v->resize(p[3],value);break;
 case 1:v->_M_fill_insert(start+p[3],p[4],value);break;
 case 2:result=(unsigned)v->erase(start+p[3],start+p[4]);break;
 case 3:v->_M_insert_overflow(start+p[3],value,tag,p[4],p[6]!=0);break;
 case 4:{const unsigned long* first=(unsigned long*)(arena+0x200)+p[3];result=(unsigned)_STL::__uninitialized_copy(first,first+p[1],(unsigned long*)(arena+0x200)+p[4],tag);break;}
 }event(4,p[0],result);++cases;}
 fclose(input);fclose(output);printf("NATIVE_ULONG_VECTOR_PASS cases=%u\n",cases);return 0;
}
