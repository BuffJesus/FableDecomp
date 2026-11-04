#include "fable_stlport_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_open_aliases.h"
#include "map_open_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall OpenMap(void*,void*,void*,unsigned);
extern "C" unsigned StreamTable[];
extern "C" unsigned __security_cookie;
extern "C" void __fastcall __security_check_cookie(unsigned);
extern "C" void* __fastcall Copy(void*,void*,void*);
extern "C" void* __fastcall Assign(void*,void*,void*);
extern "C" void __fastcall Destroy(void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events,index;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>80)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __cdecl Trap(){exit(30);}
extern "C" void __fastcall Close(void* self){record(1,self,0);at(0x20)=0;if(mode==1)at(0x1c)=0x55667788;}
extern "C" unsigned __fastcall Find(void* self,void*,const void* text){record(2,self,*(unsigned*)text);return index;}
extern "C" void* __fastcall StreamCtor(void* self){memset(self,0,24);*(void**)self=StreamTable;record(3,0,0);if(mode==2)*(unsigned short*)(arena+0x5c)=17;return self;}
extern "C" void __fastcall StreamDtor(void*){record(4,0,0);}
template<> inline void _STL::vector<char>::_M_fill_insert(char*,unsigned n,const char& value){record(5,0,n);if(n>256||value)exit(31);this->_M_start=(char*)arena+0xa00;this->_M_finish=this->_M_start+n;this->_M_end_of_storage._M_data=this->_M_finish;}
void (_STL::vector<char>::*volatile keepFill)(char*,unsigned,const char&)=&_STL::vector<char>::_M_fill_insert;
extern "C" void __fastcall Fill(void*,void*,char*,unsigned,const char&);
extern "C" void __fastcall Read(void*,void*,unsigned,void*,unsigned);
extern "C" void __fastcall MemoryOpen(void* self,void*,const void* data,unsigned n){record(7,0,n);unsigned* p=(unsigned*)self;p[1]=0;p[2]=n;p[3]=(unsigned)data;p[4]=0;p[5]=n;p[6]=(unsigned)data;}
extern "C" void __fastcall ReadMapInfo(void*,void*);
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){unsigned* p=(unsigned*)self;if(p[1]+n>92)exit(35);record(8,0,n);memcpy(out,arena+0xb00+p[1],n);p[1]+=n;p[3]=(unsigned)(arena+0xb00+p[1]);p[5]=92-p[1];if(mode==3)*(unsigned short*)(arena+0x60)=19;}
extern "C" void* __fastcall GetEntry(void* self,void*,void* result,unsigned id){record(9,self,id);*(unsigned*)result=at(0x800);((unsigned*)result)[1]=at(0x804);if(at(0x804))++*(unsigned*)at(0x804);return result;}
extern "C" void __fastcall SetEntry(void*,void*,unsigned,unsigned);
extern "C" void __fastcall Position(void* self,void*,unsigned n){if(n>256)exit(37);((unsigned*)self)[1]=n;((unsigned*)self)[3]=(unsigned)(arena+0xb00+n);((unsigned*)self)[5]=256-n;record(11,0,n);}
extern "C" void __fastcall Landscape(void*,void*,void*,void*);
extern "C" void __fastcall Detail(void*,void*,void*,void*);
extern "C" void __fastcall Palette(void* self,void*,void* stream){record(12,self,((unsigned*)stream)[1]);if(mode==5)at(4)=0;}
extern "C" void __fastcall Quad(void*,void*,void*);
extern "C" void __fastcall Types(void* self,void*,void* stream){record(18,self,((unsigned*)stream)[1]);}
extern "C" void* __cdecl ChildAllocate(unsigned n){if(n>64)exit(36);record(19,0,n);return arena+0xe00;}
extern "C" void __cdecl MapFree(void* p){record(14,p,0);}
extern "C" void __fastcall Release(void* p){record(15,p,0);if(mode==4)at(4)=0;if(mode==6)at(0x40)=(unsigned)(arena+0x880);}
void __cdecl operator delete(void* p){record(16,p,0);}
extern "C" void __cdecl Delete(void*);
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0xb3ef40:v=(unsigned)Close;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0x993b20:v=(unsigned)StreamCtor;break;case 0x123675c:v=(unsigned)StreamTable;break;case 0x4110d0:v=(unsigned)Fill;break;case 0x9cbe90:v=(unsigned)(code+896);break;case 0x994570:v=(unsigned)MemoryOpen;break;case 0xb3efa0:v=(unsigned)(code+1024);break;case 0x993ca0:v=(unsigned)Overflow;break;case 0x9d58d0:v=(unsigned)GetEntry;break;case 0xa5f6d0:v=(unsigned)(code+704);break;case 0x9c9fc0:v=(unsigned)(code+640);break;case 0x9d69a0:v=(unsigned)(code+576);break;case 0xbe03a0:v=(unsigned)(code+800);break;case 0x993bc0:v=(unsigned)Position;break;case 0xbdd0e0:v=(unsigned)(code+2688);break;case 0xbdf010:v=(unsigned)(code+3008);break;case 0xbfea0e:v=(unsigned)ChildAllocate;break;case 0xbf9290:v=(unsigned)Palette;break;case 0xbde290:v=(unsigned)(code+3264);break;case 0xbdedd0:v=(unsigned)Types;break;case 0x139c8a8:v=(unsigned)&__security_cookie;break;case 0xbfe9f9:v=(unsigned)__security_check_cookie;break;case 0x993b10:v=(unsigned)StreamDtor;break;case 0xbfea14:v=(unsigned)MapFree;break;case 0xbfe9bc:v=(unsigned)Delete;break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;}
 typedef void (__fastcall *Fn)(void*,void*,void*,unsigned);Fn fn=argv[3][0]=='r'?(Fn)code:OpenMap;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];index=p[2];events=0;for(unsigned j=0x840;j<=0x8a0;j+=0x20){at(j+4)=(unsigned)Release;at(j+8)=(unsigned)(arena+j+0xc);}
 fn(arena,0,arena+0x200,p[1]);record(17,arena,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_MAP_OPEN_PASS cases=%u\n",cases);return 0;
}
