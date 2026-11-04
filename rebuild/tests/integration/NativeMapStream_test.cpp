#include "fable_engine_internal.h"
char g_FableEmptyNarrowString_0129AAF4[1]={0}; // Unused string-stream constructor dependency.
#include "fable_stlport_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_stream_aliases.h"
#include "map_stream_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall OpenMap(void*,void*,void*,unsigned);
extern "C" unsigned StreamTable[],TypeTable[],BaseTable[],RootTable[],InputTable[],DataTable[];
namespace NEngineInternal {CEngineLocalDetailGenerator* GPLocalDetailGenerator;CEngineResourceManager* GPResourceManager;}
extern "C" unsigned __security_cookie;
extern "C" void __fastcall __security_check_cookie(unsigned);
extern "C" void* __fastcall Copy(void*,void*,void*);
extern "C" void* __fastcall Assign(void*,void*,void*);
extern "C" void __fastcall Destroy(void*);
static unsigned NativeStreamTable[11];
static unsigned char* arena;static FILE* output;static unsigned mode,events,index,names,sceneNames,objects,vectorAllocations;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,void* self,unsigned arg){if(++events>240)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,8192,output)!=8192)exit(9);}
extern "C" void __cdecl Trap(){exit(30);}
extern "C" void __fastcall Close(void* self){record(1,self,0);at(0x20)=0;if(mode==1)at(0x1c)=0x55667788;}
extern "C" unsigned __fastcall Find(void* self,void*,const void* text){record(2,self,*(unsigned*)text);return index;}
template<> inline void _STL::vector<char>::_M_fill_insert(char*,unsigned n,const char& value){record(5,0,n);if(n>768||value)exit(31);this->_M_start=(char*)arena+0x1600;this->_M_finish=this->_M_start+n;this->_M_end_of_storage._M_data=this->_M_finish;}
void (_STL::vector<char>::*volatile keepFill)(char*,unsigned,const char&)=&_STL::vector<char>::_M_fill_insert;
extern "C" void __fastcall Fill(void*,void*,char*,unsigned,const char&);
extern "C" void __fastcall Read(void*,void*,unsigned,void*,unsigned);
extern "C" void __fastcall ReadMapInfo(void*,void*);
extern "C" void __fastcall Overflow(void* self,void*,void* out,unsigned n){unsigned* p=(unsigned*)self;if(p[1]+n>768)exit(35);record(8,0,n);p[4]=p[1];memcpy(out,arena+0x1a00+p[1],n);p[1]+=n;p[3]=(unsigned)(arena+0x1a00+p[1]);p[5]=0;p[6]=(unsigned)(arena+0x1a00);if(n==52)((unsigned*)out)[-4]=0x12345678;if(mode==3)*(unsigned short*)(arena+0x60)=19;}
extern "C" void* __fastcall GetEntry(void* self,void*,void* result,unsigned id){record(9,self,id);*(unsigned*)result=at(0x800);((unsigned*)result)[1]=at(0x804);if(at(0x804))++*(unsigned*)at(0x804);return result;}
extern "C" void __fastcall SetEntry(void*,void*,unsigned,unsigned);
extern "C" void __fastcall Landscape(void*,void*,void*,void*);
extern "C" void __fastcall Detail(void*,void*,void*,void*);
extern "C" void __fastcall DefaultString(void* self){((unsigned*)self)[-1]=0x12345678;*(unsigned*)self=0x1111;record(12,0,0);if(mode==5)at(4)=0;}
extern "C" void* __fastcall ConstructString(void* self,void*,const char* text,long n){if(n!=-1||text<(char*)arena+0x1b05||text>(char*)arena+0x1c90)exit(40);unsigned size=strlen(text);if(size>8)exit(41);unsigned off;if(text<(char*)arena+0x1b80){if(names>=3)exit(42);off=0x1400+names++*32;}else off=0x1480+(sceneNames++%3)*32;at(off)=1;at(off+4)=size;memcpy(arena+off+16,text,size+1);*(unsigned*)self=(unsigned)(arena+off);record(20,(void*)text,size);return self;}
extern "C" void* __fastcall CopyString(void* self,void*,const void* other){*(unsigned*)self=*(unsigned*)other;record(21,0,*(unsigned*)other);return self;}
extern "C" void* __fastcall AssignString(void* self,void*,const void* other){*(unsigned*)self=*(unsigned*)other;record(22,self,*(unsigned*)other);return self;}
extern "C" void __fastcall DestroyString(void* self){record(23,0,*(unsigned*)self);}
extern "C" void __fastcall Quad(void*,void*,void*);
extern "C" void* __cdecl ChildAllocate(unsigned n){if(n==7){record(19,0,n);return arena+0xe00;}if(n>32||vectorAllocations>=8)exit(36);record(19,0,n);return arena+0x1100+32*vectorAllocations++;}
void* __cdecl operator new(unsigned n){if(n!=84||objects>=3)exit(43);record(24,0,n);return arena+0xa00+96*objects++;}
extern "C" void __cdecl MapFree(void* p){record(14,p,0);}
extern "C" void __fastcall Release(void* p){record(15,p,0);if(mode==4)at(4)=0;if(mode==6)at(0x40)=(unsigned)(arena+0x880);}
void __cdecl operator delete(void* p){record(16,p,0);}
extern "C" void __cdecl Delete(void*);
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,8192,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));memcpy(NativeStreamTable,StreamTable,44);NativeStreamTable[6]=(unsigned)(code+5376);NativeStreamTable[1]=(unsigned)(code+7968);
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){case 0xbfeae6:v=(unsigned)memmove;break;
 case 0xb3ef40:v=(unsigned)Close;break;case 0x9ccdc0:v=(unsigned)Find;break;case 0x993b20:v=(unsigned)(code+7872);break;case 0x123675c:v=(unsigned)NativeStreamTable;break;case 0x4110d0:v=(unsigned)Fill;break;case 0x9cbe90:v=(unsigned)(code+896);break;case 0x994570:v=(unsigned)(code+7920);break;case 0xb3efa0:v=(unsigned)(code+1024);break;case 0x993ca0:v=(unsigned)Overflow;break;case 0x9d58d0:v=(unsigned)GetEntry;break;case 0xa5f6d0:v=(unsigned)(code+704);break;case 0x9c9fc0:v=(unsigned)(code+640);break;case 0x9d69a0:v=(unsigned)(code+576);break;case 0xbe03a0:v=(unsigned)(code+800);break;case 0x993bc0:v=(unsigned)(code+7968);break;case 0xbdd0e0:v=(unsigned)(code+2688);break;case 0xbdf010:v=(unsigned)(code+3008);break;case 0xbfea0e:v=(unsigned)ChildAllocate;break;case 0xbf9290:v=(unsigned)(code+3584);break;case 0xbde290:v=(unsigned)(code+3264);break;case 0xbdedd0:v=(unsigned)(code+5632);break;case 0x139c8a8:v=(unsigned)&__security_cookie;break;case 0xbfe9f9:v=(unsigned)__security_check_cookie;break;case 0x993b10:v=(unsigned)(code+8064);break;case 0xbfea14:v=(unsigned)MapFree;break;case 0xbfe9bc:v=(unsigned)Delete;break;case 0xbf97a0:v=(unsigned)(code+4096);break;case 0xbf9420:v=(unsigned)(code+4224);break;case 0xbf95d0:v=(unsigned)(code+4320);break;case 0xbf8e50:v=(unsigned)(code+4608);break;case 0xbf8e90:v=(unsigned)(code+4672);break;case 0xbf8e20:v=(unsigned)(code+4736);break;case 0xbf8ff0:v=(unsigned)(code+4800);break;case 0xbf94d0:v=(unsigned)(code+4864);break;case 0xbf8f30:v=(unsigned)(code+5120);break;case 0xbf8f70:v=(unsigned)(code+5184);break;case 0x99e4b0:v=(unsigned)DefaultString;break;case 0x99ebf0:v=(unsigned)ConstructString;break;case 0x99ec30:v=(unsigned)CopyString;break;case 0x99efb0:v=(unsigned)AssignString;break;case 0x99eae0:v=(unsigned)DestroyString;break;case 0xbe27b0:v=(unsigned)(code+6016);break;case 0xa24280:v=(unsigned)(code+6784);break;case 0xa24290:v=(unsigned)(code+6800);break;case 0xb59c70:v=(unsigned)(code+6848);break;case 0xbe26c0:v=(unsigned)(code+7040);break;case 0xbe2680:v=(unsigned)(code+7168);break;case 0xb59f60:v=(unsigned)(code+7296);break;case 0xbdfc00:v=(unsigned)(code+7552);break;case 0x99a2f0:v=(unsigned)(code+7808);break;case 0x99a300:v=(unsigned)(code+7824);break;case 0xbfea1a:v=(unsigned)&operator new;break;case 0x12a7d94:v=(unsigned)TypeTable;break;case 0x129a7c4:v=(unsigned)BaseTable;break;case 0x1231710:v=(unsigned)RootTable;break;case 0x1436e88:v=(unsigned)&NEngineInternal::GPLocalDetailGenerator;break;case 0x1436e98:v=(unsigned)&NEngineInternal::GPResourceManager;break;case 0x129a6b0:v=(unsigned)InputTable;break;case 0x129a69c:v=(unsigned)DataTable;break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;}
 typedef void (__fastcall *Fn)(void*,void*,void*,unsigned);Fn fn=argv[3][0]=='r'?(Fn)code:OpenMap;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];index=p[2];events=names=sceneNames=objects=vectorAllocations=0;NEngineInternal::GPLocalDetailGenerator=(CEngineLocalDetailGenerator*)(arena+0x1500);NEngineInternal::GPResourceManager=(CEngineResourceManager*)(arena+0xf00);for(unsigned j=0x840;j<=0x8a0;j+=0x20){at(j+4)=(unsigned)Release;at(j+8)=(unsigned)(arena+j+0xc);}
 fn(arena,0,arena+0x200,p[1]);record(17,arena,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_MAP_STREAM_PASS cases=%u\n",cases);return 0;
}
