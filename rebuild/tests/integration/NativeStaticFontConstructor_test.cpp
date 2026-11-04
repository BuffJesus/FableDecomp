#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_font_instances.h"
#include "fable_font_data_banks.h"
#include "fable_data_input_stream.h"
#include "fable_gfuninitialise.h"
#include "fable_surface.h"
#define FABLE_STUBS_NO_MEMORY_DATA_INPUT_STREAM
#include "test_support/data_input_stream_stubs.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned row;
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"STATIC_FONT_CTOR_FAILURE row=%u line=%d\n",row,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
fable_i32 g_CCharStringInstanceCount_013BD800=0;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
struct Block{void* pointer;unsigned size;bool array;};static Block blocks[256];static unsigned allocations,frees;
static void* Allocate(unsigned size,bool array){unsigned char* p=(unsigned char*)malloc(size+32);CHECK(p);memset(p,0xcd,size+32);unsigned i=0;while(i<256 && blocks[i].pointer)++i;CHECK(i<256);blocks[i].pointer=p+16;blocks[i].size=size;blocks[i].array=array;++allocations;return p+16;}
static void Release(void* p,bool array){if(!p)return;unsigned i=0;while(i<256 && blocks[i].pointer!=p)++i;CHECK(i<256 && blocks[i].array==array);unsigned char* bytes=(unsigned char*)p;for(unsigned n=0;n<16;++n)CHECK(bytes[int(n)-16]==0xcd && bytes[blocks[i].size+n]==0xcd);blocks[i].pointer=0;++frees;free(bytes-16);}
void* operator new(size_t n){return Allocate(n,false);}void operator delete(void* p){Release(p,false);}void* operator new[](size_t n){return Allocate(n,true);}void operator delete[](void* p){Release(p,true);}
static unsigned char payload[8192],before[8192];static unsigned payloadSize,bankMode,seed,pixelBytes;static bool observing;static CStaticFontBank* observed;static unsigned phase,systemCalls,copyCalls;
struct Event{unsigned kind,a,b,c;};static Event events[16];static unsigned eventCount;
static void Record(unsigned k,unsigned a=0,unsigned b=0,unsigned c=0){CHECK(eventCount<16);events[eventCount].kind=k;events[eventCount].a=a;events[eventCount].b=b;events[eventCount++].c=c;}
__declspec(align(16)) static unsigned char bankStorage[sizeof(CFontDataBank)],systemStorage[sizeof(CSystemManager)],displayStorage[sizeof(CDisplayManager)];
static CFontDataBank* bank=(CFontDataBank*)bankStorage;static CSystemManager* fixtureSystem=(CSystemManager*)systemStorage;static CDisplayManager* display=(CDisplayManager*)displayStorage;
unsigned long CBankFile::GetEntryDataSize(unsigned long index)const{CHECK(observing && this==bank && index==0x42 && phase==0);phase=1;Record(1,payloadSize);return payloadSize;}
void CBankFile::ReadEntryData(unsigned long index,unsigned long offset,void* data,unsigned long size){CHECK(observing && this==bank && index==0x42 && !offset && size==payloadSize && phase==1);for(unsigned i=0;i<size;++i)CHECK(((unsigned char*)data)[i]==0);memcpy(data,payload,size);phase=2;Record(2,size);}
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){if(observing){CHECK(phase==2);++systemCalls;Record(3,systemCalls);}return fixtureSystem;}
void CPixelFormat::Initialise(long r,long g,long b,long a,bool sign){CHECK(observing && r==8 && g==8 && b==8 && a==8 && !sign && systemCalls==1 && phase==2);FormatIndex=17;Record(4,17);}
bool CDisplayManager::GetBestCompressedAlphaTextureFormat(unsigned long depth,CPixelFormat& format){CHECK(observing && this==display && depth==16 && format.FormatIndex==-1 && systemCalls==2 && phase==2);format.FormatIndex=23+seed;phase=3;Record(5,depth,format.FormatIndex);return (seed&1)!=0;}
bool CTexture::Initialise(const C2DExtentsI& dimensions,long levels,const CPixelFormat& format,unsigned long flags,ESurfacePool pool,bool dynamic,bool mipmaps){CHECK(observing && phase==3 && !PD3DTexture && AllocationSource==0 && ByteLength==0x0dcdcdcd && (const void*)&dimensions==(const void*)&observed->TextureSize_X && dimensions.X==observed->TextureSize_X && dimensions.Y==observed->TextureSize_Y && levels==1 && format.FormatIndex==23+seed && !flags && pool==SURFACE_POOL_MANAGED && !dynamic && !mipmaps && observed->PTexture==(CTexture*)0xa5a5a5a5);PD3DTexture=(IDirect3DTexture9*)0x11223344;ByteLength=0x222;AllocationSource=1;phase=4;Record(6,dimensions.X,dimensions.Y,format.FormatIndex);return !(seed&2);}
CSurface CTexture::GetSurfaceLevel(long level)const{CHECK(observing && phase==4 && this==observed->PTexture && !level && PD3DTexture==(IDirect3DTexture9*)0x11223344);CSurface surface;surface.PD3DSurface=(IDirect3DSurface9*)0x55667788;surface.PAllocatedMemory=(void*)0x12340000;phase=5;Record(7,level);return surface;}
void CSurface::CopyFromMemory(const C2DBoxI* dest,const void* source,const C2DBoxI* box,const CPixelFormat& format,long pitch,unsigned long filter){CHECK(observing && phase==5 && dest && source && box && dest!=box && !memcmp(dest,box,sizeof(C2DBoxI)) && box->left==0 && box->top==0 && box->right==observed->TextureSize_X && box->bottom==observed->TextureSize_Y && format.FormatIndex==17 && pitch==observed->TextureSize_X*4 && filter==0xffffffff && PD3DSurface==(IDirect3DSurface9*)0x55667788);for(unsigned i=0;i<pixelBytes;++i)CHECK(((unsigned char*)source)[i]==(unsigned char)(seed+i*13));for(unsigned j=pixelBytes;j<pixelBytes+18;++j)CHECK(((unsigned char*)source)[j]==0);phase=6;++copyCalls;Record(8,pitch,pixelBytes,filter);}
CSurface::CSurface(const CSurface& source):PD3DSurface(source.PD3DSurface),AllocationSource(source.AllocationSource),PAllocatedMemory(source.PAllocatedMemory){}
void CSurface::Uninitialise(){if(observing){CHECK((phase==5 || phase==6) && PD3DSurface==(IDirect3DSurface9*)0x55667788);if(phase==6){phase=7;Record(9);}else Record(10);}PD3DSurface=0;}
void CTexture::Uninitialise(){CHECK(!observing);PD3DTexture=0;AllocationSource=0;}
void CStaticFontBank::PrepareToRender(bool)const{CHECK(false);}void CStaticFontBank::Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{CHECK(false);}
void CDataInputStream::SetPosition(unsigned long){CHECK(false);}CFileDataInputStream::~CFileDataInputStream(){CHECK(false);}template<>void _STL::vector<char>::resize(unsigned,const char&){CHECK(false);}
extern "C" void* __fastcall Construct(CStaticFontBank*,void*,CFontDataBank*,unsigned long);
extern "C" unsigned long StaticVtable[];
#pragma comment(linker,"/alternatename:@Construct@16=??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z")
#pragma comment(linker,"/alternatename:_StaticVtable=??_7CStaticFontBank@@6B@")
typedef void*(__fastcall* Constructor)(CStaticFontBank*,void*,CFontDataBank*,unsigned long);
static void Append(const void* p,unsigned n){CHECK(payloadSize+n<=sizeof(payload));memcpy(payload+payloadSize,p,n);payloadSize+=n;}
static void Word(unsigned n){Append(&n,4);}
static const char* names[]={"","Atlas","glyphs/native/split"};
static unsigned Banks(){return bankMode==0?0:(bankMode==1?1:3);}
static unsigned Index(unsigned i){unsigned values[]={0,17,1023};return values[i];}
static unsigned Count(unsigned i){return bankMode==2?0:(bankMode==4?62-i:(1+(seed+i)%3));}
static void Make(){payloadSize=0;Append(names[seed%3],(unsigned)strlen(names[seed%3])+1);Word(12+seed);Word(0x80000000+seed);unsigned char italics=(unsigned char)(seed%3==0?0:(seed%3==1?1:0x80));Append(&italics,1);Word(99+seed);Word(4+(seed%3)*4);Word(2+(seed%2)*2);Word(seed?32:0);Word(65535-seed);Word(Banks());
 for(unsigned i=0;i<Banks();++i){Word(Index(i));Word(0x10000+2+i);Word(0x10000+Count(i));for(unsigned j=0;j<Count(i);++j){unsigned bits[]={0x7f812345+seed+j,0x80000000,0x3f800000+j,1+seed};Append(bits,16);unsigned short shorts[]={(unsigned short)(0x8000+seed),(unsigned short)(3+j),(unsigned short)(0xffff-j)};Append(shorts,6);}}
 Word(pixelBytes+18);unsigned char header[18];memset(header,0xf1,18);Append(header,18);for(unsigned k=0;k<pixelBytes;++k){unsigned char b=(unsigned char)(seed+k*13);Append(&b,1);}memcpy(before,payload,sizeof(payload));}
struct Result{unsigned char bytes[sizeof(CStaticFontBank)];unsigned glyphHash,allocations,frees,count;Event events[16];};
static Result Run(Constructor ctor){CHECK(allocations==frees && !g_CCharStringInstanceCount_013BD800);unsigned oldA=allocations,oldF=frees;struct Guard{unsigned char before[16];unsigned char font[sizeof(CStaticFontBank)];unsigned char after[16];}storage;memset(&storage,0xa5,sizeof(storage));observed=(CStaticFontBank*)storage.font;phase=systemCalls=copyCalls=eventCount=0;memset(events,0,sizeof(events));observing=true;CHECK(ctor(observed,0,bank,0x42)==observed);observing=false;
 CHECK(phase==7 && systemCalls==2 && copyCalls==1 && !memcmp(before,payload,sizeof(payload)) && g_CCharStringInstanceCount_013BD800==1);CHECK(*(unsigned*)observed==(unsigned)StaticVtable && !strcmp((const char*)observed->Name,names[seed%3]));CHECK(observed->FontHeight==12+seed && observed->Weight==(long)(0x80000000+seed) && observed->Italics==(seed%3!=0) && observed->MaxHeight==99+seed && observed->TextureSize_X==4+(seed%3)*4 && observed->TextureSize_Y==2+(seed%2)*2 && observed->MinChar==(seed?32:0) && observed->MaxChar==65535-seed && observed->TotalNoGlyphs==(long)0xa5a5a5a5 && observed->PManager==(CFontManager*)0xa5a5a5a5);
 Result result;memset(&result,0,sizeof(result));memcpy(result.bytes,observed,sizeof(*observed));CStaticFontBank* normalized=(CStaticFontBank*)result.bytes;memset(&normalized->Name,0,sizeof(normalized->Name));normalized->PTexture=(CTexture*)0x54585452;unsigned hash=2166136261;
 for(unsigned i=0;i<1024;++i){unsigned entry=0;while(entry<Banks() && Index(entry)!=i)++entry;CStaticFontGlyphBank& glyph=observed->GlyphBanks[i];if(entry==Banks()){CHECK(!glyph.Glyphs && !glyph.FirstGlyph && !glyph.NoGlyphs);}else{CHECK(glyph.Glyphs && glyph.FirstGlyph==2+entry && glyph.NoGlyphs==Count(entry));unsigned block=0;while(block<256 && blocks[block].pointer!=glyph.Glyphs)++block;CHECK(block<256 && blocks[block].array && blocks[block].size==Count(entry)*sizeof(CGlyph));for(unsigned j=0;j<Count(entry);++j){const unsigned char* p=(const unsigned char*)&glyph.Glyphs[j];CHECK(p[22]==0xcd && p[23]==0xcd);unsigned bits[]={0x7f812345+seed+j,0x80000000,0x3f800000+j,1+seed};CHECK(!memcmp(p,bits,16));unsigned short shorts[]={(unsigned short)(0x8000+seed),(unsigned short)(3+j),(unsigned short)(0xffff-j)};CHECK(!memcmp(p+16,shorts,6));for(unsigned k=0;k<24;++k)hash=(hash^p[k])*16777619;}normalized->GlyphBanks[i].Glyphs=(CGlyph*)0x474c5950;}}
 for(unsigned i=0;i<16;++i)CHECK(storage.before[i]==0xa5 && storage.after[i]==0xa5);result.glyphHash=hash;result.count=eventCount;memcpy(result.events,events,sizeof(events));observed->~CStaticFontBank();CHECK(!g_CCharStringInstanceCount_013BD800 && allocations==frees);result.allocations=allocations-oldA;result.frees=frees-oldF;return result;}
int main(int argc,char** argv){CHECK(argc==1 || argc==3);Constructor original=0;unsigned char* code=0;if(argc==3){code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code);FILE* f=fopen(argv[1],"rb");CHECK(f && fread(code,1,2197,f)==2197 && fgetc(f)==EOF);CHECK(!fclose(f));f=fopen(argv[2],"rb");CHECK(f);unsigned count;CHECK(fread(&count,4,1,f)==1 && count<80);for(unsigned i=0;i<count;++i){unsigned row[3];CHECK(fread(row,4,3,f)==3 && row[0]+4<=2197);*(unsigned*)(code+row[0])=row[1]-(row[2]?(unsigned)(code+row[0]+4):0);}CHECK(fgetc(f)==EOF && !fclose(f));original=(Constructor)code;}
 memset(systemStorage,0,sizeof(systemStorage));systemStorage[4]=1;fixtureSystem->PDisplayManager.Data=display;
 for(seed=0;seed<6;++seed)for(bankMode=0;bankMode<5;++bankMode)for(unsigned bytes=0;bytes<3;++bytes){pixelBytes=(4+(seed%3)*4)*(2+(seed%2)*2)*4+bytes*16;Make();Result candidate=Run(Construct);if(original){Result native=Run(original);CHECK(!memcmp(&candidate,&native,sizeof(Result)));}++row;}
 if(code)CHECK(VirtualFree(code,0,0x8000));printf("STATIC_FONT_CTOR_PASS cases=%u original=%u allocations=%u frees=%u\n",row,original?row:0,allocations,frees);return 0;}
