#include "fable_indexed_array.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mode,count,duplicate,copies;static unsigned* valuePointer;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void require(bool v){if(!v)exit(21);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);fwrite(arena,1,0x1000,output);}
static void replace(unsigned n,unsigned capacity){word(0)=(unsigned)(arena+0x400);word(4)=word(0)+n*4;word(8)=word(0)+capacity*4;}
CCharString::CCharString(const CCharString& source){unsigned value=*(const unsigned*)&source;event(1,(unsigned)this>=(unsigned)arena&&(unsigned)this<(unsigned)arena+0x1000?(unsigned)this:0,value);++copies;if(mode==1&&copies==1)replace(count+1,count+3);else if(mode==4&&copies==2)replace(count+1,count+3);*(unsigned*)this=value;}
CCharString::~CCharString(){event(3,*(unsigned*)this);*(unsigned*)this=0;if(mode==3)replace(count,count);}
namespace _STL {
template<> pair<map<CCharString,long>::iterator,bool> map<CCharString,long>::_Rep_type::insert_unique(const map<CCharString,long>::value_type& value){require(this==(void*)(arena+12));event(2,*(const unsigned*)&value.first,value.second,duplicate);if(!duplicate)word(0xa00)=value.second;if(mode==2){replace(count,count+3);*valuePointer^=0x101;}return pair<map<CCharString,long>::iterator,bool>(map<CCharString,long>::iterator((_Rb_tree_node_base*)(arena+0xb00)),!duplicate);}
template<> void vector<CCharString>::_M_insert_overflow(CCharString* position,const CCharString& value,const __false_type&,size_t fill,bool atend){require(this==(void*)arena&&fill==1&&atend);event(4,(unsigned)position,*(const unsigned*)&value,1);unsigned n=size(),v=*(const unsigned*)&value;memmove(arena+0x600,_M_start,n*4);word(0x600+n*4)=v;_M_start=(CCharString*)(arena+0x600);_M_finish=_M_start+n+1;_M_end_of_storage._M_data=_M_start+n+2;}
}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[5],cases=0;while(fread(p,4,5,input)==5){if(fread(arena,1,0x1000,input)!=0x1000)return 5;count=p[0];duplicate=p[3];mode=p[4];copies=0;valuePointer=(unsigned*)(arena+(p[2]?0x200:0x900));((CIndexedArray<CCharString>*)arena)->push_back(*(CCharString*)valuePointer);event(5);++cases;}
 fclose(input);fclose(output);printf("NATIVE_INDEXED_CHAR_APPEND_PASS cases=%u\n",cases);return 0;
}
