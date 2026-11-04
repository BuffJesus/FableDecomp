#include <stdio.h>
#include "fable_quick_draw_info.h"
#include "fable_stlport_vector.h"
namespace _STL { template<> CQuickDrawTriInfo* vector<CQuickDrawTriInfo>::erase(CQuickDrawTriInfo*,CQuickDrawTriInfo*); }
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;static FILE* output;static unsigned mutation;
static unsigned& word(unsigned offset){return *(unsigned*)(arena+offset);}
static void event(unsigned kind,unsigned receiver=0,unsigned source=0)
{unsigned args[]={kind,receiver,source};fwrite(args,4,3,output);fwrite(arena,1,0x1000,output);}
static void mutate()
{if(mutation)for(unsigned i=0;i<8;++i)word(0x100+i*60+28)^=0x73450123;}
CWideString& CWideString::operator=(const CWideString& other)
{event(1,(unsigned)this,(unsigned)&other);mutate();storage_=other.storage_;return *this;}
CWideString::~CWideString(){event(2,(unsigned)this);mutate();storage_=0;}
static unsigned long __stdcall release(void* receiver){event(3,(unsigned)receiver);mutate();return 7;}
static unsigned long __stdcall addref(void* receiver){event(4,(unsigned)receiver);mutate();return 7;}
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 word(0x8004)=(unsigned)&addref;word(0x8008)=(unsigned)&release;
 unsigned first,last,cases=0;
 while(fread(&mutation,4,1,input)==1)
 {
  if(fread(&first,4,1,input)!=1 || fread(&last,4,1,input)!=1 || fread(arena,1,0x1000,input)!=0x1000)return 5;
  _STL::vector<CQuickDrawTriInfo>* v=(_STL::vector<CQuickDrawTriInfo>*)arena;
  CQuickDrawTriInfo* records=(CQuickDrawTriInfo*)(arena+0x100);
  event(5,(unsigned)v->erase(records+first,records+last));++cases;
 }
 if(ferror(input))return 6;fclose(input);if(fclose(output))return 7;
 printf("NATIVE_QUICK_DRAW_ERASE_PASS cases=%u\n",cases);return 0;
}
