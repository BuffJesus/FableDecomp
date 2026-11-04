#include "fable_shader_render_manager.h"
#include "fable_shader_resource.h"
#include "fable_indexed_array.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static const char* names[]={"PSCONST_INPUT_FACTOR_0","PSCONST_INPUT_FACTOR_1","PSCONST_OUTPUT_FACTOR","PSCONST_MAX_FOG_ALPHA","PSCONST_SHADOW_FACTOR_AND_FADE_COLOUR","PSCONST_USER_0","PSCONST_USER_1","PSCONST_USER_2","PSCONST_USER_3"};
static unsigned char* arena;static FILE* output;static unsigned mode,resources,tablecount,resourceCalls,nameCalls;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void require(bool v){if(!v)exit(21);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);fwrite(arena,1,0x4000,output);}
namespace _STL {
template<> void vector<const CVertexShaderConstantLayout*>::resize(size_t count,const CVertexShaderConstantLayout* const& value){require(this==(void*)(arena+12)&&count==1&&!value);event(1,(unsigned)this,1);_M_start=(const CVertexShaderConstantLayout**)(arena+0x500);_M_finish=_M_start+1;_M_end_of_storage._M_data=_M_start+4;if(mode==1){word(0x3a8)=(unsigned)(arena+0x700);word(0x3ac)=(unsigned)(arena+0x700)+resources*8;}}
template<> void vector<C4DVector>::_M_fill_insert(C4DVector* position,size_t count,const C4DVector& value){require(this==(void*)(arena+0x3cc));unsigned zero[4]={0,0,0,0};require(memcmp(&value,zero,16)==0);event(6,(unsigned)position,count);for(unsigned i=0;i<count;++i)memset(position+i,0,16);_M_finish+=count;if(mode==5){_M_start=(C4DVector*)(arena+0x2000);_M_finish=_M_start+9;_M_end_of_storage._M_data=_M_start+16;}}
template<> C4DVector* vector<C4DVector>::erase(C4DVector* first,C4DVector* last){require(this==(void*)(arena+0x3cc));event(7,(unsigned)first,(unsigned)last);_M_finish=first;if(mode==5){_M_start=(C4DVector*)(arena+0x2000);_M_finish=_M_start+9;_M_end_of_storage._M_data=_M_start+16;}return first;}
}
void CShaderResource::Initialise(CShaderRenderManager& manager){require(&manager==(void*)arena);event(2,(unsigned)this,(unsigned)&manager);ShaderRenderManager=&manager;++resourceCalls;if(mode==2)word(0x3ac)=word(0x3a8)+8;else if(mode==3&&resourceCalls==1)word(0x3ac)=word(0x3a8)+(resources+1)*8;}
CCharString::CCharString(const char* literal,long length){unsigned index=0;while(index<9&&strcmp(literal,names[index]))++index;require(index<9&&length==-1);event(3,index,0xffffffff);*(unsigned*)this=0xabc00000+index;}
CCharString::~CCharString(){event(5,*(unsigned*)this);*(unsigned*)this=0;}
template<> void CIndexedArray<CCharString>::push_back(const CCharString& name){unsigned value=*(const unsigned*)&name;require(this==(void*)(arena+0x3b4)&&value==0xabc00000+nameCalls);event(4,value);++nameCalls;*(unsigned*)Array._M_finish=value;++Array._M_finish;if(mode==4){word(0x3cc)=(unsigned)(arena+0x2000);word(0x3d0)=(unsigned)(arena+0x2000)+tablecount*16;word(0x3d4)=(unsigned)(arena+0x2100);}}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[5],cases=0;while(fread(p,4,5,input)==5){if(fread(arena,1,0x4000,input)!=0x4000)return 5;resources=p[0];tablecount=p[2];mode=p[4];resourceCalls=nameCalls=0;((CShaderRenderManager*)arena)->Initialise((CRenderManager*)(arena+0x3000),(IDirect3DDevice9*)(arena+0x3100));require(nameCalls==9);event(8);++cases;}
 fclose(input);fclose(output);printf("NATIVE_SHADER_INIT_PASS cases=%u\n",cases);return 0;
}
