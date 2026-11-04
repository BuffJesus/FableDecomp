#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_font_instances.h"
#include "fable_system_manager.h"
#include "fable_display_viewport.h"
#include "fable_render_state_defaults.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
extern unsigned long FableRenderDefaultValue_01396dc8;
static unsigned row,path,seed,calls,fvfCalls,status;
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"FONT_PREPARE_FAILURE row=%u path=%u seed=%u line=%d\n",row,path,seed,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
__declspec(align(16)) static unsigned char systemBytes[sizeof(CSystemManager)],displayBytes[sizeof(CDisplayManager)],managerBytes[sizeof(CRenderManager)+32],fontBytes[sizeof(CStaticFontBank)+32];
static CSystemManager* fixtureSystem=(CSystemManager*)systemBytes;
static CDisplayManager* display=(CDisplayManager*)displayBytes;
static CRenderManager* render=(CRenderManager*)(managerBytes+16);
static CStaticFontBank* font=(CStaticFontBank*)(fontBytes+16);
static unsigned char initial[sizeof(CRenderManager)],expected[sizeof(CRenderManager)],beforeFVF[sizeof(CRenderManager)];
static unsigned offsets[26]={0x28b8,0x291c,0x2930,0x28cc,0x28e0,0x28a4,0x282c,0x2890,0x2868,0x2944,0x2a20,0x30c4,0x3164,0x2e44,0x2ee4,0x2f84,0x3664,0x35c4,0x3204,0x3344,0x33e4,0x32a4,0x3484,0x3524,0x3218,0x32b8};
static unsigned long* values[26];static unsigned long literalOne=1,literalZero=0,literalEight=8;
static unsigned long* globals[13];static unsigned long initialGlobals[13],expectedGlobals[13];
struct CRenderStateManagerTestAccess{
 static unsigned long* Mip(){return CRenderStateManager::D3DTextureMipMapModeTable;}
 static unsigned long* Alpha(){return CRenderStateManager::D3DAlphaFuncModeTable;}
 static unsigned long* Operation(){return CRenderStateManager::D3DTextureOperationModeTable;}
 static void Counts(unsigned stack,unsigned dirty,unsigned mask){render->StateStackSize=stack;render->StateUpdateListSize=dirty;render->CurrentBookmarkMask=mask;}
 static void Model(unsigned index){CRenderStateManager::CStateInfo* info=(CRenderStateManager::CStateInfo*)((unsigned char*)render+offsets[index]);unsigned long value=*values[index];if(info->DesiredState==value)return;if(!(info->BookmarkMask & render->CurrentBookmarkMask)){info->BookmarkMask|=render->CurrentBookmarkMask;CHECK(render->StateStackSize<1024);render->StateStack[render->StateStackSize].State=info;render->StateStack[render->StateStackSize].Value=info->DesiredState;++render->StateStackSize;}info->DesiredState=value;if(!info->DirtyListFlag){CHECK(render->StateUpdateListSize<256);info->DirtyListFlag=true;render->StateUpdateList[render->StateUpdateListSize++]=info;}}
};
static void Mutate(){if(seed&1){*values[18]^=0x13579bdf;CRenderStateManager::CStateInfo* info=(CRenderStateManager::CStateInfo*)((unsigned char*)render+offsets[17]);info->DesiredState^=0x12345678;}}
static void* deviceTable[119];static void* deviceObject[1]={deviceTable};
static HRESULT __stdcall SetFVF(void* self,unsigned format){CHECK(self==deviceObject && format==0x144 && !fvfCalls);CHECK(!memcmp(render,beforeFVF,sizeof(beforeFVF)));++fvfCalls;Mutate();return (HRESULT)status;}
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){++calls;CHECK(calls==1);return fixtureSystem;}
static void Tables(){
 values[0]=&literalOne;values[1]=&FableRenderDefaultValue_01396f78;values[2]=&FableRenderDefaultValue_01396f7c;values[3]=&literalOne;values[4]=&literalEight;values[5]=CRenderStateManagerTestAccess::Alpha()+ALPHA_TEST_GREATER_EQUAL;values[6]=&FableRenderDefaultValue_01396fa4;values[7]=&FableRenderDefaultValue_01396fb0;values[8]=&literalOne;values[9]=&literalZero;values[10]=&literalOne;values[11]=values[12]=&FableRenderDefaultValue_01396e5c;values[13]=CRenderStateManagerTestAccess::Mip();values[14]=values[15]=&FableRenderDefaultValue_01396dc8;values[16]=&FableRenderDefaultValue_01396fcc;values[17]=&literalZero;values[18]=values[21]=CRenderStateManagerTestAccess::Operation()+TEXTURE_OP_MODULATE;values[19]=values[22]=&FableRenderDefaultValue_01396ef0;values[20]=values[23]=&FableRenderDefaultValue_01396eec;values[24]=values[25]=CRenderStateManagerTestAccess::Operation()+TEXTURE_OP_DISABLE;
 unsigned n=0;for(unsigned i=0;i<26;++i){if(values[i]==&literalZero || values[i]==&literalOne || values[i]==&literalEight)continue;bool found=false;for(unsigned j=0;j<n;++j)if(globals[j]==values[i])found=true;if(!found){CHECK(n<13);globals[n++]=values[i];}}CHECK(n==13);
}
static unsigned char* Original(const char* name,const char* patchName){FILE* file=fopen(name,"rb");CHECK(file);unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code && fread(code,1,2672,file)==2672 && fgetc(file)==EOF && !fclose(file));file=fopen(patchName,"rb");unsigned count;CHECK(file && fread(&count,4,1,file)==1);for(unsigned i=0;i<count;++i){unsigned patch[3];CHECK(fread(patch,4,3,file)==3 && patch[0]<=2668);*(unsigned*)(code+patch[0])=patch[1]-(patch[2]?(unsigned)(code+patch[0]+4):0);}CHECK(fgetc(file)==EOF && !fclose(file));return code;}
int main(int argc,char** argv){CHECK(argc==1 || argc==3);unsigned char* code=argc==3?Original(argv[1],argv[2]):0;typedef void(CStaticFontBank::*Method)(bool)const;Method original=0;CHECK(sizeof(original)==4);memcpy(&original,&code,4);Tables();
 memset(systemBytes,0,sizeof(systemBytes));memset(displayBytes,0,sizeof(displayBytes));memset(fontBytes,0xa5,sizeof(fontBytes));fixtureSystem->PDisplayManager.Data=display;CRenderManager* pointer=render;memcpy(&display->PRenderManager,&pointer,4);deviceTable[89]=(void*)SetFVF;
 unsigned long nativeGlobals[13];for(unsigned i=0;i<13;++i)nativeGlobals[i]=*globals[i];const unsigned masks[]={0,1,2,0x80000000,0x10001,0xffffffff};
 for(seed=0;seed<32;++seed)for(unsigned enabled=0;enabled<2;++enabled)for(unsigned maskIndex=0;maskIndex<6;++maskIndex)for(unsigned capacity=0;capacity<3;++capacity)for(unsigned fail=0;fail<2;++fail){status=fail?0x80004005:0;
  memset(managerBytes,0xa5,sizeof(managerBytes));render->PD3DDevice=(IDirect3DDevice9*)deviceObject;
  for(unsigned i=0;i<13;++i){initialGlobals[i]=seed<8?nativeGlobals[i]:seed<16?nativeGlobals[i]^0x13579bdf:seed<24?0:0xffffffff;*globals[i]=initialGlobals[i];}
  unsigned mask=masks[maskIndex];CRenderStateManagerTestAccess::Counts(capacity==0?0:capacity==1?51:998,capacity==0?0:capacity==1?13:230,mask);
  for(unsigned i=0;i<26;++i){CRenderStateManager::CStateInfo* info=(CRenderStateManager::CStateInfo*)((unsigned char*)render+offsets[i]);info->DesiredState=((seed+i)%3)==0?*values[i]:*values[i]^(0x11111111+(seed+i)*37);info->BookmarkMask=((seed+i)%4)==0?mask:((seed+i)%4)==1?~mask:0;info->DirtyListFlag=((seed+i)&1)!=0;}
  memcpy(initial,render,sizeof(initial));for(unsigned i=0;i<17;++i)CRenderStateManagerTestAccess::Model(i);memcpy(beforeFVF,render,sizeof(beforeFVF));if(enabled){Mutate();for(unsigned i=17;i<26;++i)CRenderStateManagerTestAccess::Model(i);}memcpy(expected,render,sizeof(expected));for(unsigned i=0;i<13;++i)expectedGlobals[i]=*globals[i];
  for(path=0;path<(code?2:1);++path){memcpy(render,initial,sizeof(initial));for(unsigned i=0;i<13;++i)*globals[i]=initialGlobals[i];calls=fvfCalls=0;unsigned control=_controlfp(0,0),fpstatus=_statusfp();if(path)(font->*original)(enabled!=0);else font->CStaticFontBank::PrepareToRender(enabled!=0);CHECK(calls==1 && fvfCalls==enabled && control==_controlfp(0,0) && fpstatus==_statusfp());if(memcmp(render,expected,sizeof(expected))){unsigned char* actual=(unsigned char*)render;for(unsigned i=0;i<sizeof(expected);++i)if(actual[i]!=expected[i]){fprintf(stderr,"STATE_DIFF offset=%x actual=%02x expected=%02x\n",i,actual[i],expected[i]);break;}CHECK(false);}for(unsigned i=0;i<13;++i)CHECK(*globals[i]==expectedGlobals[i]);for(unsigned i=0;i<sizeof(fontBytes);++i)CHECK(fontBytes[i]==0xa5);for(unsigned i=0;i<16;++i)CHECK(managerBytes[i]==0xa5 && managerBytes[sizeof(CRenderManager)+16+i]==0xa5);
  }++row;
 }
 for(unsigned i=0;i<13;++i)*globals[i]=nativeGlobals[i];if(code)CHECK(VirtualFree(code,0,0x8000));printf("STATIC_FONT_PREPARE_SCRATCH_PASS cases=%u paths=%u\n",row,row*(code?2:1));return 0;
}
