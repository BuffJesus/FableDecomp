#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_map_aliases.h"
#include "common_map_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall OpenCommon(void*,void*,long);
static unsigned char* arena;static FILE* output;static unsigned mode,byteSize,initialRemaining,allocations,names,nodes,events,opens;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,unsigned a,unsigned b){if(++events>64)exit(10);unsigned h[]={kind,a,b};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" unsigned __fastcall Size(void* self,void*,unsigned index){if(self!=arena+0x34||index!=3)exit(20);record(1,(unsigned)self,index);return byteSize;}
extern "C" void* __cdecl CommonAlloc(unsigned size){unsigned offset=byteSize&&allocations==0?0x800:0x900;unsigned expected=offset==0x800?byteSize:24;if(size!=expected||allocations++>1)exit(21);record(2,size,offset);return arena+offset;}
extern "C" void __cdecl CommonFree(void* p){if(p!=arena+0x800&&p!=arena+0x900)exit(22);record(12,(unsigned)p,0);*(unsigned*)p=0xeeeeeeee;}
extern "C" void __fastcall ReadBank(void* self,void*,unsigned index,unsigned offset,void* dest,unsigned size){if(self!=arena+0x34||index!=3||offset||size!=byteSize)exit(23);record(3,index,size);memcpy(dest,arena+0xa00,size);}
extern "C" void* __fastcall Stream(void* self,void*,const void* data,unsigned size){unsigned* s=(unsigned*)self;s[0]=0x123675c;s[1]=0;s[2]=size;s[3]=(unsigned)data;s[4]=0;s[5]=size?initialRemaining:0;s[6]=(unsigned)data;record(4,size,s[5]);return self;}
extern "C" void __fastcall Overflow(void* self,void*,void* dest,long size){unsigned* s=(unsigned*)self;if(size!=4||s[1]>60)exit(24);memcpy(dest,arena+0xa00+s[1],4);s[1]+=4;s[3]=(unsigned)(arena+0xa00+s[1]);s[5]=0;record(5,s[1],*(unsigned*)dest);}
extern "C" void* __fastcall ReadName(void* self,void*,void* result){unsigned* s=(unsigned*)self;if(names>=3)exit(25);unsigned ptr=(unsigned)(arena+0xb00+32*names++);*(unsigned*)ptr=1;*(unsigned*)result=ptr;s[1]+=4;s[3]+=4;s[5]=mode==2?0:s[5]-4;record(6,ptr,s[1]);return result;}
extern "C" void* __fastcall Copy(void* self,void*,const void* other){unsigned ptr=*(unsigned*)other;*(unsigned*)self=ptr;if(ptr)++*(unsigned*)ptr;record(7,ptr,0);return self;}
extern "C" void __fastcall Destroy(void* self){unsigned ptr=*(unsigned*)self;if(ptr)--*(unsigned*)ptr;record(8,ptr,0);}
extern "C" void* __fastcall Insert(void* self,void*,void* result,const void* value){unsigned* t=(unsigned*)self;const unsigned* v=(const unsigned*)value;if(t[0]!=(unsigned)(arena+0x900))exit(26);record(9,v[0],v[1]);
 unsigned node=(unsigned)(arena+0x980+24*nodes);if(mode!=6){++nodes;++t[1];*(unsigned*)v[0]+=1;*(unsigned*)(node+16)=v[0];*(unsigned*)(node+20)=v[1];at(0x904)=node;if(nodes==1)at(0x908)=node;at(0x90c)=node;}
 *(unsigned*)result=node;*((unsigned char*)result+4)=mode!=6;if(mode==3)at(0x24)=at(0x20)+4;return result;
}
extern "C" void __fastcall Erase(void* self,void*,void* root){unsigned* t=(unsigned*)self;record(11,(unsigned)root,t[1]);for(unsigned n=0;n<nodes;++n)--*(unsigned*)at(0x990+24*n);}
extern "C" void __fastcall OpenMap(void* self,void*,void* bank,void* stream,void* positions){if(bank!=arena+0x34||*(unsigned*)positions!=(unsigned)(arena+0x900))exit(27);unsigned* s=(unsigned*)stream;record(10,(unsigned)self,s[1]);if(mode==4&&opens++==0)at(0x24)=at(0x20)+12;if(mode==5)s[1]+=4;}
extern "C" void __fastcall StreamDestroy(void* self){record(13,((unsigned*)self)[1],0);}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned n=0;n<19;++n){unsigned off=retailFixups[n][0],v=0;switch(retailFixups[n][1]){
 case 0x9cc240:v=(unsigned)Size;break;case 0xbfea0e:v=(unsigned)CommonAlloc;break;case 0xbfea14:v=(unsigned)CommonFree;break;case 0x9cc2a0:v=(unsigned)ReadBank;break;case 0x994930:v=(unsigned)Stream;break;case 0x993ca0:v=(unsigned)Overflow;break;case 0x995d30:v=(unsigned)ReadName;break;case 0x99ec30:v=(unsigned)Copy;break;case 0x99eae0:v=(unsigned)Destroy;break;case 0x431606:v=(unsigned)Insert;break;case 0x43020a:v=(unsigned)Erase;break;case 0xb41e50:v=(unsigned)OpenMap;break;case 0x993b10:v=(unsigned)StreamDestroy;break;default:return 8;}
 *(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void (__fastcall *Fn)(void*,void*,long);Fn fn=argv[3][0]=='r'?(Fn)code:OpenCommon;
 unsigned p[3],cases=0;while(fread(p,4,3,in)==3){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];byteSize=p[1];initialRemaining=p[2];allocations=names=nodes=events=opens=0;fn(arena,0,3);record(14,0,0);++cases;}
 fclose(in);fclose(output);printf("NATIVE_COMMON_MAP_HEADER_PASS cases=%u\n",cases);return 0;
}
