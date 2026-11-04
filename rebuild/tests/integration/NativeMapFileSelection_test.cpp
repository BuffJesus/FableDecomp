#include "engine/CEngineWorldMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "fable_engine_internal.h"
#include "fable_progress_display.h"
#include <stdio.h>
#include <stdlib.h>
#include "map_file_aliases.h"
#include "map_file_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall SelectMapFile(void*,void*,const CWideString&);
extern "C" void __fastcall ConstructText(void*,void*,const char*,long);
extern "C" void __fastcall DestroyText(void*);
extern "C" void __fastcall CloseMaps(void*);
extern "C" void __fastcall DisposePool(void*);
extern "C" void __fastcall AssignWide(void*,void*,const void*);
extern "C" void __fastcall OpenMaps(void*,void*,long);
extern "C" void __fastcall LoadWater(void*);
static unsigned char* arena;static FILE* output;static unsigned mode,stage;
static const char* texts[]={"SetStaticMapFileForUse: CloseStaticMapFile","SetStaticMapFileForUse: EnablePoolAllocation","SetStaticMapFileForUse: OpenStaticMaps","SetStaticMapFileForUse: LoadWaterData"};
static void record(unsigned kind,unsigned arg){unsigned h[]={kind,arg,(unsigned)NEngineInternal::GPLandscapeRenderer};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,8192,output)!=8192)exit(9);}
CCharString::CCharString(const char* text,long length){unsigned id=0;while(id<4&&strcmp(text,texts[id]))++id;if(id==4||length!=-1)exit(10);*(unsigned*)this=id+1;record(1,id);}
CCharString::~CCharString(){record(3,*(unsigned*)this);}
void __fastcall NProgressDisplay::DisplayProgress(const CCharString& text,float fraction,bool forceRender,bool isDebug){if(fraction!=-1.0f||forceRender||isDebug)exit(11);stage=*(unsigned*)&text;record(2,stage);if(mode==1)NEngineInternal::GPLandscapeRenderer=(CEngineLandscapeRenderer*)(arena+0x1000);if(mode==2)*(unsigned*)(arena+0x400)=stage*123;}
void CEngineWorldMap::CloseStaticMapFile(){record(4,(unsigned)this);StaticMapBankName=*(CWideString*)(arena+0x404);if(mode==3)NEngineInternal::GPLandscapeRenderer=(CEngineLandscapeRenderer*)(arena+0x1000);}
void CEngineLandscapeMemoryManager::FreeDisposalList(){record(5,(unsigned)this);if(mode==4)*(unsigned*)(arena+0x400)=0xabcd;}
CWideString& CWideString::operator=(const CWideString& other){record(6,(unsigned)this);*(unsigned*)this=*(unsigned*)&other;if(mode==5)*(unsigned*)(arena+0x1b0)=0xfeed;return *this;}
void CEngineWorldMap::OpenStaticMaps(CEngineMap::STATIC_MAP_OPEN_STATE state){if(state!=CEngineMap::STATIC_MAP_OPEN_STATE_OPEN_FOR_USE)exit(13);record(7,(unsigned)this);if(mode==6)StaticMapBuildStatus=0x8765;}
void CEngineWorldMap::LoadWaterData(){record(8,(unsigned)this);if(mode==7)StaticMapBuildStatus=0x1234;}
int main(int argc,char** argv){
 if(argc!=4||NEngineInternal::GPLandscapeRenderer)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;
  switch(retailFixups[i][1]){case 0x99ebf0:v=(unsigned)ConstructText;break;case 0x99eae0:v=(unsigned)DestroyText;break;case 0x9e9f40:v=(unsigned)NProgressDisplay::DisplayProgress;break;case 0xb40000:v=(unsigned)CloseMaps;break;case 0xbda070:v=(unsigned)(code+256);break;case 0xbd9ce0:v=(unsigned)DisposePool;break;case 0x99b7d0:v=(unsigned)AssignWide;break;case 0xb42750:v=(unsigned)OpenMaps;break;case 0xb41fa0:v=(unsigned)LoadWater;break;case 0x1436ea8:v=(unsigned)&NEngineInternal::GPLandscapeRenderer;break;case 0x12a202c:v=(unsigned)texts[0];break;case 0x12a1ffc:v=(unsigned)texts[1];break;case 0x12a1fd4:v=(unsigned)texts[2];break;case 0x12a1fac:v=(unsigned)texts[3];break;default:return 8;}
  *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;
 }
 typedef void (__fastcall *Select)(void*,void*,const CWideString&);Select run=argv[3][0]=='r'?(Select)code:SelectMapFile;
 unsigned p[3],cases=0;
 while(fread(p,4,3,in)==3){if(fread(arena,1,8192,in)!=8192)return 5;mode=p[0];NEngineInternal::GPLandscapeRenderer=(CEngineLandscapeRenderer*)(arena+p[2]);
  for(unsigned j=0;j<2;++j){run(arena,0,*(CWideString*)(arena+p[1]));record(9,j);}++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_MAP_FILE_SELECTION_PASS cases=%u\n",cases);return 0;
}
