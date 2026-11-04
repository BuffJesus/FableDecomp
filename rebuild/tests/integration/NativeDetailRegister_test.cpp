#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEngineLocalDetailGenerator.h"
#include "detail_register_aliases.h"
#include "detail_register_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" const void* __fastcall Register(void*,void*,const void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events,checks,allocation,fail;static int match;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>50)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
static void swap(){unsigned bytes=at(0x10)-at(0xc),capacity=at(0x14)-at(0xc);at(0xc)=(unsigned)(arena+0x400);at(0x10)=at(0xc)+bytes;at(0x14)=at(0xc)+capacity;}
extern "C" bool __fastcall Equivalent(void* self,void*,const void* other){record(1,self,(unsigned)other);bool yes=(int)checks++==match;if(mode==1)swap();if(mode==2)at(0x10)=at(0xc);if(mode==3&&yes)*(unsigned*)(at(0xc)+(checks-1)*4)=(unsigned)(arena+0x740);return yes;}
void* __cdecl operator new(unsigned n){if(n!=84||allocation>=2)exit(21);record(2,0,n);if(mode==5)at(0x804)^=0x55;unsigned i=allocation++;return fail?0:arena+0xa00+i*0x100;}
extern "C" void* __fastcall Copy(void* self,void*,const void* other){memcpy(self,other,84);record(3,self,(unsigned)other);if(mode==4)swap();return self;}
extern "C" void __fastcall Grow(void* self,void*,void* pos,const void* value,const void*,unsigned n,bool end){if(self!=arena+0xc||pos!=(void*)at(0x10)||n!=1||!end)exit(22);unsigned bytes=at(0x10)-at(0xc);if(bytes>64)exit(23);record(4,self,*(unsigned*)value);memmove(arena+0xc00,(void*)at(0xc),bytes);at(0xc)=(unsigned)(arena+0xc00);at(0x10)=at(0xc)+bytes;at(0x14)=at(0xc)+64;*(unsigned*)at(0x10)=*(unsigned*)value;at(0x10)+=4;if(mode==6)at(0xa04)^=0x33;}
namespace _STL {template<> inline void vector<CLocalDetailObjectCollectionType*>::_M_insert_overflow(CLocalDetailObjectCollectionType** p,CLocalDetailObjectCollectionType* const& x,const __true_type& tag,size_t n,bool end){Grow(this,0,p,&x,&tag,n,end);}}
int main(int argc,char** argv){
 void (_STL::vector<CLocalDetailObjectCollectionType*>::*volatile keep)(CLocalDetailObjectCollectionType**,CLocalDetailObjectCollectionType* const&,const _STL::__true_type&,size_t,bool)=&_STL::vector<CLocalDetailObjectCollectionType*>::_M_insert_overflow;
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<4;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0xbe2680:v=(unsigned)Equivalent;break;case 0xbe26c0:v=(unsigned)Copy;break;case 0xbfea1a:v=(unsigned)&operator new;break;case 0xb59f60:v=(unsigned)Grow;break;default:return 8;}*(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef const void* (__fastcall *Fn)(void*,void*,const void*);Fn fn=argv[3][0]=='r'?(Fn)code:Register;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];match=(int)p[1];fail=p[2];events=allocation=0;for(unsigned i=0;i<2;++i){checks=0;const void* result=fn(arena,0,arena+0x800);record(5,(void*)result,i);}++cases;}
 fclose(in);fclose(output);printf("NATIVE_DETAIL_REGISTER_PASS cases=%u\n",cases);return 0;
}
