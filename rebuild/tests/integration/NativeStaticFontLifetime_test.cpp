#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_font_instances.h"
#include "fable_gfuninitialise.h"
#include "fable_surface_types.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned caseIndex;
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"STATIC_FONT_LIFETIME_FAILURE case=%u line=%d\n",caseIndex,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
fable_i32 g_CCharStringInstanceCount_013BD800=0;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
struct Block{void* p;unsigned size;bool array;unsigned kind,index;};static Block blocks[256];static unsigned allocated,freed;
static bool observing;static CStaticFontBank* observedFont;static unsigned glyphDeletes,textureDeletes,fontDeletes,comReleases,systemCalls;
static unsigned expectedGlyphs,previousGlyphIndex;static unsigned char savedFont[sizeof(CStaticFontBank)];static unsigned systemMode;
struct Event{unsigned kind,index;};static Event events[256];static unsigned eventCount;
static void EventAt(unsigned kind,unsigned index){if(observing){CHECK(eventCount<256);events[eventCount].kind=kind;events[eventCount++].index=index;}}
static unsigned Find(void* p){unsigned i=0;while(i<256 && blocks[i].p!=p)++i;CHECK(i<256);return i;}
static void* Allocate(size_t size,bool array){unsigned char* raw=(unsigned char*)malloc(size+32);CHECK(raw);memset(raw,0xcd,size+32);unsigned i=0;while(i<256 && blocks[i].p)++i;CHECK(i<256);blocks[i].p=raw+16;blocks[i].size=(unsigned)size;blocks[i].array=array;blocks[i].kind=blocks[i].index=0;++allocated;return raw+16;}
static void Release(void* p,bool array){
 if(!p)return;unsigned i=Find(p);Block block=blocks[i];CHECK(block.array==array);unsigned char* bytes=(unsigned char*)p;for(unsigned n=0;n<16;++n)CHECK(bytes[int(n)-16]==0xcd && bytes[block.size+n]==0xcd);
 if(observing){
  if(block.kind==1){CHECK(!array && glyphDeletes==expectedGlyphs && g_CCharStringInstanceCount_013BD800==1);++fontDeletes;memcpy(savedFont,p,sizeof(savedFont));EventAt(5,0);}
  if(block.kind==2){CHECK(array && observedFont->GlyphBanks[block.index].Glyphs==p && (!glyphDeletes || block.index>previousGlyphIndex));previousGlyphIndex=block.index;++glyphDeletes;EventAt(1,block.index);}
  if(block.kind==3){CTexture* texture=(CTexture*)p;CHECK(!array && glyphDeletes==expectedGlyphs && !texture->PD3DTexture && (texture->AllocationSource==0 || !comReleases));++textureDeletes;EventAt(4,0);}
 }
 blocks[i].p=0;++freed;free(bytes-16);
}
void* operator new(size_t n){return Allocate(n,false);}void operator delete(void* p){Release(p,false);}void* operator new[](size_t n){return Allocate(n,true);}void operator delete[](void* p){Release(p,true);}
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)];
static CSystemManager* borrowedSystem=reinterpret_cast<CSystemManager*>(systemStorage);
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){if(observing){++systemCalls;EventAt(2,systemCalls);}return systemMode?borrowedSystem:0;}
static unsigned long __stdcall ReleaseCOM(void*){if(observing){CHECK(glyphDeletes==expectedGlyphs);++comReleases;EventAt(3,0);}return 7;}
static void* comTable[3]={0,0,(void*)&ReleaseCOM};static void* comObject[1]={comTable};
CStaticFontBank::CStaticFontBank(CFontDataBank*,unsigned long){}
void CStaticFontBank::PrepareToRender(bool)const{CHECK(false);}
void CStaticFontBank::Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{CHECK(false);}
extern "C" void* __fastcall StaticScalar(CStaticFontBank*,void*,unsigned);
extern "C" void __fastcall FontNameDtor(CCharString*,void*);
extern "C" void __fastcall FontBaseDtor(CBaseClassNonCopyable*,void*);
extern "C" bool __fastcall InitValid(const CInitBaseClass*,void*);
extern "C" void __fastcall TextureUninitialise(CTexture*,void*);
extern "C" unsigned long StaticVtable[],FontVtable[],BaseVtable[];
#pragma comment(linker,"/alternatename:@StaticScalar@12=??_GCStaticFontBank@@UAEPAXI@Z")
#pragma comment(linker,"/alternatename:@FontNameDtor@8=??1CCharString@@QAE@XZ")
#pragma comment(linker,"/alternatename:@FontBaseDtor@8=??1CBaseClassNonCopyable@@UAE@XZ")
#pragma comment(linker,"/alternatename:@InitValid@8=?IsValid@CInitBaseClass@@QBE_NXZ")
#pragma comment(linker,"/alternatename:@TextureUninitialise@8=?Uninitialise@CTexture@@QAEXXZ")
#pragma comment(linker,"/alternatename:_StaticVtable=??_7CStaticFontBank@@6B@")
#pragma comment(linker,"/alternatename:_FontVtable=??_7CFontBank@@6B@")
#pragma comment(linker,"/alternatename:_BaseVtable=??_7CBaseClass@@6B@")
typedef void*(__fastcall* Scalar)(CStaticFontBank*,void*,unsigned);
struct Result{unsigned char bytes[sizeof(CStaticFontBank)];unsigned glyphs,textures,fonts,releases,systems,count;Event events[256];unsigned allocations,frees;};
static Result Run(Scalar scalar,unsigned context,unsigned glyphMode,unsigned nameKind,unsigned textureKind,unsigned flags){
 CHECK(allocated==freed && !g_CCharStringInstanceCount_013BD800);unsigned allocationsBefore=allocated,freesBefore=freed;Result result;memset(&result,0,sizeof(result));
 {CCharString share(nameKind==2?"shared-static-font":"");CStaticFontBank* font=new CStaticFontBank(0,0);blocks[Find(font)].kind=1;observedFont=font;CHECK(sizeof(*font)==0x2034 && g_CCharStringInstanceCount_013BD800==2);
  font->FontHeight=0x12345678;font->PManager=(CFontManager*)0x76543210;
  if(nameKind==1){CCharString name("exclusive-static-font");font->Name=name;}else if(nameKind==2)font->Name=share;
  for(unsigned i=0;i<1024;++i){font->GlyphBanks[i].FirstGlyph=(unsigned short)(i&63);font->GlyphBanks[i].NoGlyphs=(unsigned short)(64-(i&63));font->GlyphBanks[i].Glyphs=0;}
  expectedGlyphs=0;
  for(unsigned i=0;i<1024;++i){bool present=(glyphMode==1 && (i==0 || i==1023)) || (glyphMode==2 && i>=17 && i<=19) || (glyphMode==3 && i%17==0);if(present){CGlyph* glyphs=new CGlyph[1+(i%64)];font->GlyphBanks[i].Glyphs=glyphs;unsigned block=Find(glyphs);blocks[block].kind=2;blocks[block].index=i;++expectedGlyphs;}}
  CTexture* texture=textureKind?new CTexture:0;font->PTexture=texture;if(texture){blocks[Find(texture)].kind=3;texture->PD3DTexture=textureKind==2?(IDirect3DTexture9*)comObject:0;texture->ByteLength=0x24681357;texture->AllocationSource=textureKind==2?0x13572468:0;}
  memset(systemStorage,0,sizeof(systemStorage));systemMode=context;systemStorage[4]=(unsigned char)(context>=3);borrowedSystem->PDisplayManager.Data=(context==2 || context==4)?(CDisplayManager*)0x11223340:0;
  glyphDeletes=textureDeletes=fontDeletes=comReleases=systemCalls=previousGlyphIndex=eventCount=0;memset(events,0,sizeof(events));memset(savedFont,0,sizeof(savedFont));observing=true;
  void* returned=scalar(font,0,flags);CHECK(returned==font && g_CCharStringInstanceCount_013BD800==1 && glyphDeletes==expectedGlyphs);
  bool releaseTexture=context==4 && textureKind!=0;CHECK(textureDeletes==(releaseTexture?1:0) && comReleases==(releaseTexture && textureKind==2?1:0) && fontDeletes==((flags&1)?1:0) && systemCalls==(context==0?1:(context<=2?2:3)));
  if(flags&1)memcpy(result.bytes,savedFont,sizeof(result.bytes));else memcpy(result.bytes,font,sizeof(result.bytes));
  const CStaticFontBank* dead=reinterpret_cast<const CStaticFontBank*>(result.bytes);CHECK(*(unsigned long*)result.bytes==(unsigned long)BaseVtable && dead->FontHeight==0x12345678 && !*reinterpret_cast<CCharStringData* const*>(&dead->Name) && dead->PManager==(CFontManager*)0x76543210);
  for(unsigned i=0;i<1024;++i)CHECK(!dead->GlyphBanks[i].Glyphs && dead->GlyphBanks[i].FirstGlyph==(i&63) && dead->GlyphBanks[i].NoGlyphs==64-(i&63));
  CHECK(dead->PTexture==(context==4?0:texture));if(dead->PTexture)reinterpret_cast<CStaticFontBank*>(result.bytes)->PTexture=(CTexture*)0x54585452;
  if(nameKind==2)CHECK(share.GetLength()==18 && !strcmp((const char*)share,"shared-static-font"));
  result.glyphs=glyphDeletes;result.textures=textureDeletes;result.fonts=fontDeletes;result.releases=comReleases;result.systems=systemCalls;result.count=eventCount;memcpy(result.events,events,sizeof(events));
  observing=false;if(!(flags&1))::operator delete(font);if(texture && !releaseTexture)delete texture;observedFont=0;
 }
 CHECK(allocated==freed && !g_CCharStringInstanceCount_013BD800);result.allocations=allocated-allocationsBefore;result.frees=freed-freesBefore;return result;
}
static void Relative(unsigned char* code,unsigned at,void* target){*(unsigned long*)(code+at)=(unsigned long)target-((unsigned long)code+at+4);}
int main(int argc,char** argv){
 CHECK(argc==1 || argc==3);unsigned char* code=0;unsigned char* scalar=0;Scalar original=0;
 if(argc==3){code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);scalar=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code && scalar);
 FILE* f=fopen(argv[1],"rb");CHECK(f && fread(code,1,143,f)==143 && fgetc(f)==EOF);CHECK(!fclose(f));f=fopen(argv[2],"rb");CHECK(f && fread(scalar,1,30,f)==30 && fgetc(f)==EOF);CHECK(!fclose(f));
 *(unsigned long*)(code+7)=(unsigned long)StaticVtable;*(unsigned long*)(code+124)=(unsigned long)FontVtable;typedef void(__cdecl* Delete)(void*);Delete arrayDelete=&::operator delete[],objectDelete=&::operator delete;
 Relative(code,27,(void*)arrayDelete);Relative(code,47,(void*)&GFGetSystemManager);Relative(code,56,(void*)&GFGetSystemManager);Relative(code,63,(void*)InitValid);Relative(code,72,(void*)&GFGetSystemManager);Relative(code,96,(void*)TextureUninitialise);Relative(code,102,(void*)objectDelete);Relative(code,129,(void*)FontNameDtor);Relative(code,139,(void*)FontBaseDtor);Relative(scalar,4,code);Relative(scalar,17,(void*)objectDelete);
 original=reinterpret_cast<Scalar>(scalar);}
 unsigned originalCases=0;unsigned flags[]={0,1,2,3,0x80,0x81};
 for(unsigned context=0;context<5;++context)for(unsigned glyph=0;glyph<4;++glyph)for(unsigned name=0;name<3;++name)for(unsigned texture=0;texture<3;++texture)for(unsigned flag=0;flag<6;++flag){Result candidate=Run(StaticScalar,context,glyph,name,texture,flags[flag]);if(original){Result native=Run(original,context,glyph,name,texture,flags[flag]);CHECK(!memcmp(&native,&candidate,sizeof(Result)));++originalCases;}++caseIndex;}
 CHECK(caseIndex==1080 && allocated==freed && !g_CCharStringInstanceCount_013BD800);if(code)CHECK(VirtualFree(code,0,0x8000));if(scalar)CHECK(VirtualFree(scalar,0,0x8000));printf("NATIVE_STATIC_FONT_LIFETIME_PASS cases=%u original=%u paths=%u contexts=5 glyph_patterns=4 names=3 textures=3 flags=6 allocations=%u frees=%u\n",caseIndex,originalCases,caseIndex+originalCases,allocated,freed);return 0;
}
