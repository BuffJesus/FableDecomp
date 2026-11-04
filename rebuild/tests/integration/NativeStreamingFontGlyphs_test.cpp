// Streaming font glyph lookup: actual CStreamingFontBank::FindGlyph / GlyphLookup / GetStringExtents (the
// module unit is compiled into this one so its file statics can be seeded) against the transplanted retail
// 0x009A4160 / 0x009A1140 / 0x009A14E0, from identical states. Generated glyph data (gen.py): two fonts,
// raw and LZO-compressed glyph banks, LZO-compressed glyph-data chunks, adjustments. Retail
// lzo1x_decompress_safe 0x00C08170 is transplanted and used by both modes (library boundary).
// The gate compiles this file in one unit after the streaming font module source (or a mutated copy of it).
// InitializeStreamingFontTextureCache (retail 0x009A3640) is compared first: it reads the streaming bank's
// glyph-data header, chunk index and adjustments (served by fixture StreamGlyph/ReadData boundaries from the
// generated data) and creates the font texture cache through a scripted CTexture::Initialise (A8 / A4R4G4B4 /
// best-alpha fallbacks); the glyph tests then run on the statics it loaded.
#include <new>
#include "fable_counted_pointer_inline.h"
#include "fable_stlport_vector_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned op,mode;
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"GLYPH_FAILURE op=%u mode=%u line=%d\n",op,mode,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
// Retail float constants the transplanted code reads (the gate checks the image values).
extern "C" {float GlyphHundredth=0.01f,GlyphZero=0.0f,GlyphHalf=0.5f,GlyphUnsignedBias=4294967296.0f,GlyphOne=1.0f,GlyphTexelU=1.0f/1024.0f,GlyphTexelV=1.0f/512.0f;}
#pragma comment(linker,"/include:_GlyphOne")
#pragma comment(linker,"/include:_GlyphTexelU")
#pragma comment(linker,"/include:_GlyphTexelV")
// Render: the shader manager's two enable flags (+0x390/+0x391) and the retail block's own copies of Render's
// static locals (index table and its build-once flag); the candidate uses its own.
extern "C" {unsigned char FixtureShaderManager[0x400];bool RenderBuildIndicesOriginal=true;unsigned short RenderIndicesOriginal[384];}
#pragma comment(linker,"/alternatename:?g_CShaderRenderManager_013BC470@@3VCShaderRenderManager@@A=_FixtureShaderManager")
#pragma comment(linker,"/include:_RenderBuildIndicesOriginal")
#pragma comment(linker,"/include:_RenderIndicesOriginal")
#pragma comment(linker,"/include:_GlyphHundredth")
#pragma comment(linker,"/include:_GlyphZero")
#pragma comment(linker,"/include:_GlyphHalf")
#pragma comment(linker,"/include:_GlyphUnsignedBias")
// Frame counter through the actual system/display manager path (GFGetSystemManager()->PDisplayManager->FrameCounter).
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)],displayStorage[sizeof(CDisplayManager)];
static CSystemManager* fixtureSystem=(CSystemManager*)systemStorage;static CDisplayManager* display=(CDisplayManager*)displayStorage;
__declspec(noinline) CSystemManager* FABLE_FASTCALL GFGetSystemManager(){return fixtureSystem;}
#pragma comment(linker,"/include:?GFGetSystemManager@@YIPAVCSystemManager@@XZ")
// Transplanted retail block: lzo, FindGlyph, GlyphLookup, GetStringExtents; patch kinds as the UI gate.
enum{LZO_AT=0,FIND_AT=1024,LOOKUP_AT=1280,EXTENTS_AT=2304,INIT_AT=4096,CTOR_AT=6144,DTOR_AT=7456,ENTRIES_AT=7648,PRELOAD_AT=8192,INSERT_AT=9184,UPDATE_AT=9296,TEXTURE_UPDATE_AT=9472,READ_TEXT_AT=10240,PREPARE_AT=11264,RENDER_AT=13568,BLOCK=17408};
static unsigned char* code;
// Patch kind 3: the module's file statics are not in the link map; the fixture supplies their addresses
// (index in the high byte of the patch value, addend in the low 24 bits).
static void* DataAddress(unsigned index);
typedef int (__cdecl* LzoFn)(const unsigned char*,unsigned long,unsigned char*,unsigned long*,void*);
extern "C" int __cdecl lzo1x_decompress_safe(const unsigned char* s,unsigned long n,unsigned char* d,unsigned long* dn,void* w){return ((LzoFn)(code+LZO_AT))(s,n,d,dn,w);}
static void Original(const char* name,const char* patchName){FILE* input=fopen(name,"rb");CHECK(input);code=(unsigned char*)VirtualAlloc(0,BLOCK,0x3000,0x40);CHECK(code && fread(code,1,BLOCK,input)==BLOCK && fgetc(input)==EOF && !fclose(input));input=fopen(patchName,"rb");unsigned count;CHECK(input && fread(&count,4,1,input)==1);for(unsigned i=0;i<count;++i){unsigned patch[3];CHECK(fread(patch,4,3,input)==3 && patch[0]<=BLOCK-4 && patch[2]<=3);unsigned target=patch[2]==2?(unsigned)(code+patch[1]):patch[2]==3?(unsigned)DataAddress(patch[1]>>24)+(patch[1]&0xFFFFFF):patch[1];if(patch[2]==3)patch[2]=0;*(unsigned*)(code+patch[0])=target-(patch[2]?(unsigned)(code+patch[0]+4):0);}CHECK(fgetc(input)==EOF && !fclose(input));}
typedef CStreamingGlyphLookup* (CStreamingFontBank::*LookupFn)(unsigned short) const;
typedef const CStreamingGlyph* (CStreamingFontBank::*FindFn)(unsigned short) const;
typedef C2DExtentsF (CStreamingFontBank::*ExtentsFn)(const wchar_t*,bool) const;
struct Probe:CStreamingFontBank{static LookupFn Lookup(){return &Probe::GlyphLookup;}static FindFn Find(){return &Probe::FindGlyph;}};
template<class T> static T Member(unsigned at){T f;CHECK(sizeof(f)==4);unsigned char* p=code+at;memcpy(&f,&p,4);return f;}
// Boundaries shared by both modes.
// Engine allocator: one 512 KiB block for the glyph pixel cache.
CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
static unsigned char allocatorObject[16];static unsigned char* textureMemory;static unsigned createCalls,enterCalls,leaveCalls,allocCalls;
__declspec(noinline) void FABLE_FASTCALL CMemoryAllocator::Create(){CHECK(!g_FableMemoryAllocator_013CAA90);++createCalls;g_FableMemoryAllocator_013CAA90=(CMemoryAllocator*)allocatorObject;}
__declspec(noinline) void FABLE_FASTCALL CMemoryAllocator::EnterSynchronisation(){CHECK(enterCalls==leaveCalls);++enterCalls;}
__declspec(noinline) void FABLE_FASTCALL CMemoryAllocator::LeaveSynchronisation(){++leaveCalls;CHECK(enterCalls==leaveCalls);}
__declspec(noinline) void* CMemoryAllocator::InternalAllocate(long size,unsigned long category,bool flag){CHECK(this==(CMemoryAllocator*)allocatorObject && size==0x80000 && !category && !flag && enterCalls==leaveCalls+1);++allocCalls;return textureMemory;}
// Texture creation: scripted per call (bit k of textureScript = call k succeeds); every argument recorded.
struct TextureCall{long x,y,levels,format;unsigned long flags;long pool;bool dynamic,mips;};
static unsigned textureScript,textureCount;static TextureCall textureCalls[4];
__declspec(noinline) bool CTexture::Initialise(const C2DExtentsI& d,long levels,const CPixelFormat& f,unsigned long flags,ESurfacePool pool,bool dynamic,bool mips){CHECK(this==&GFontTextureCache && textureCount<4);TextureCall c;memset(&c,0,sizeof(c));c.x=d.X;c.y=d.Y;c.levels=levels;c.format=f.FormatIndex;c.flags=flags;c.pool=(long)pool;c.dynamic=dynamic;c.mips=mips;textureCalls[textureCount]=c;return ((textureScript>>textureCount++)&1)!=0;}
__declspec(noinline) void CTexture::Uninitialise(){}
// Texture-cache lock: a 1024x512 texel buffer (up to 4 bytes per texel) with a padded pitch.
enum{TEXEL_PITCH=1024*4+64};static unsigned char* texelMemory;static bool textureLocked;static unsigned textureLocks;
__declspec(noinline) CSurfaceLockedRect CTexture::Lock(unsigned long level,unsigned long flags){CHECK(this==&GFontTextureCache && !level && !flags && !textureLocked);textureLocked=true;++textureLocks;CSurfaceLockedRect r;r.Dimensions=C2DExtentsI(1024,512);r.Pitch=TEXEL_PITCH;r.PMem=texelMemory;return r;}
__declspec(noinline) void CTexture::Unlock(unsigned long level){CHECK(this==&GFontTextureCache && !level && textureLocked);textureLocked=false;}
#pragma comment(linker,"/include:?Lock@CTexture@@QAE?AVCSurfaceLockedRect@@KK@Z")
#pragma comment(linker,"/include:?Unlock@CTexture@@QAEXK@Z")
#pragma comment(linker,"/include:?Uninitialise@CTexture@@QAEXXZ")
// Display: the best 32-bit alpha texture format.
static long bestIndex;static unsigned bestCalls;
__declspec(noinline) bool CDisplayManager::GetBestAlphaTextureFormat(unsigned long depth,CPixelFormat& f){CHECK(this==display && depth==32 && f.FormatIndex==-1);++bestCalls;f.FormatIndex=bestIndex;return true;}
// Streaming bank: header (glyph count, adjustment count, memory offset, data size, per-chunk count), glyph data,
// chunk index, adjustments; synchronous reads only.
__declspec(align(16)) static unsigned char streamBankStorage[sizeof(CStreamingFontDataBank)];static CStreamingFontDataBank* const streamBank=(CStreamingFontDataBank*)streamBankStorage;
static unsigned char* streamBlob;static unsigned long streamSize;static unsigned streamCalls,readCalls;
__declspec(noinline) // Glyph pixel reads (Preload): the cell gets an offset-derived pattern; the read either completes at once (null
// request) or stays in flight as a scripted request until the fixture finishes it.
enum{ASYNC_POOL=1024};
static unsigned char asyncObjects[ASYNC_POOL][8];__declspec(align(4)) static unsigned char asyncInfoStorage[ASYNC_POOL][sizeof(CCPPointerInfo)];static CCPPointerInfo* const asyncInfos=(CCPPointerInfo*)asyncInfoStorage;static bool asyncFinished[ASYNC_POOL],asyncDeleted[ASYNC_POOL];static unsigned asyncUsed,pixelReads;
static bool inFlight;
static void FABLE_FASTCALL AsyncDelete(void* data){const unsigned i=(unsigned)((unsigned char(*)[8])data-asyncObjects);CHECK(i<asyncUsed && !asyncDeleted[i]);asyncDeleted[i]=true;}
__declspec(noinline) bool CBankFileAsyncData::IsFinished()const{const unsigned i=(unsigned)((const unsigned char(*)[8])this-asyncObjects);CHECK(i<asyncUsed && !asyncDeleted[i]);return asyncFinished[i];}
#pragma comment(linker,"/include:?IsFinished@CBankFileAsyncData@@QBE_NXZ")
// The retail block calls these out of line; keep them linked whatever a (mutated) unit inlines.
#pragma comment(linker,"/include:??4?$CCountedPointer@VCBankFileAsyncData@@@@QAEAAV0@ABV0@@Z")
#pragma comment(linker,"/include:??1?$CCountedPointer@VCBankFileAsyncData@@@@QAE@XZ")
template CCountedPointer<CBankFileAsyncData>::~CCountedPointer();
template CCountedPointer<CBankFileAsyncData>& CCountedPointer<CBankFileAsyncData>::operator=(const CCountedPointer<CBankFileAsyncData>&);
CCountedPointer<CBankFileAsyncData> CStreamingFontDataBank::StreamGlyph(void* d,unsigned long o,unsigned long n,bool sync){CHECK(this==streamBank);
 if(!sync){CHECK(d>=textureMemory && (unsigned char*)d+n<=textureMemory+0x80000 && (n==0x400 || n==0x1000));for(unsigned long i=0;i<n;++i)((unsigned char*)d)[i]=(unsigned char)((o>>3)+i*7);++pixelReads;
  CCountedPointer<CBankFileAsyncData> request;if(inFlight){CHECK(asyncUsed<ASYNC_POOL);const unsigned k=asyncUsed++;asyncFinished[k]=false;asyncDeleted[k]=false;new(&asyncInfos[k]) CCPPointerInfo(&AsyncDelete,asyncObjects[k]);request.Data=(CBankFileAsyncData*)asyncObjects[k];request.Info=&asyncInfos[k];}
  return request;}
 CHECK(n==4 && o==4*streamCalls && o+n<=streamSize);memcpy(d,streamBlob+o,n);++streamCalls;return CCountedPointer<CBankFileAsyncData>();}
__declspec(noinline) void CStreamingFontDataBank::ReadData(void* d,unsigned long o,unsigned long n){CHECK(this==streamBank && d && o+n<=streamSize);memcpy(d,streamBlob+o,n);++readCalls;}
// Boundaries must stay out of line (the module is in this unit) and linked for the retail block's patches.
#pragma comment(linker,"/include:?Create@CMemoryAllocator@@KIXXZ")
#pragma comment(linker,"/include:?EnterSynchronisation@CMemoryAllocator@@KIXXZ")
#pragma comment(linker,"/include:?LeaveSynchronisation@CMemoryAllocator@@KIXXZ")
#pragma comment(linker,"/include:?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z")
#pragma comment(linker,"/include:?Initialise@CTexture@@QAE_NABVC2DExtentsI@@JABVCPixelFormat@@KW4ESurfacePool@@_N3@Z")
#pragma comment(linker,"/include:?GetBestAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z")
#pragma comment(linker,"/include:?StreamGlyph@CStreamingFontDataBank@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@PAXKK_N@Z")
#pragma comment(linker,"/include:?ReadData@CStreamingFontDataBank@@QAEXPAXKK@Z")
#pragma comment(linker,"/include:?_M_fill_insert@?$vector@EV?$allocator@E@_STL@@@_STL@@QAEXPAEIABE@Z")
#pragma comment(linker,"/include:??_U@YAPAXI@Z")
#pragma comment(linker,"/include:??3@YAXPAX@Z")
#pragma comment(linker,"/include:?GD3DFormats@@3QBVCD3DFormatInfo@@B")
// Font records (CStreamingFontBank's constructor reads them as bank entries 1 and 2).
static unsigned char* records[2];static unsigned long recordSizes[2];static unsigned recordReads;
__declspec(noinline) unsigned long CBankFile::GetEntryDataSize(unsigned long index)const{CHECK(this==(const CBankFile*)streamBank && (index==1 || index==2));if(getenv("GLYPH_TRACE"))fprintf(stderr,"size %lu\n",recordSizes[index-1]);return recordSizes[index-1];}
__declspec(noinline) void CBankFile::ReadEntryData(unsigned long index,unsigned long offset,void* d,unsigned long n){CHECK(this==(CBankFile*)streamBank && (index==1 || index==2) && !offset && n==recordSizes[index-1] && d);memcpy(d,records[index-1],n);++recordReads;if(getenv("GLYPH_TRACE"))fprintf(stderr,"read %lu\n",n);}
#pragma comment(linker,"/include:?GetEntryDataSize@CBankFile@@QBEKK@Z")
#pragma comment(linker,"/include:?ReadEntryData@CBankFile@@QAEXKKPAXK@Z")
// Allocator teardown (UninitializeStreamingFontTextureCache): the glyph pixel cache goes back; no shutdown.
bool g_FableAllocatorShutdownRequested_013CAA94=false;static unsigned deallocCalls;
__declspec(noinline) void CMemoryAllocator::InternalDeallocate(void* p){CHECK(this==(CMemoryAllocator*)allocatorObject && p==textureMemory && enterCalls==leaveCalls+1);++deallocCalls;}
__declspec(noinline) unsigned long CMemoryAllocator::GetNoAllocations(){CHECK(false);return 0;}
__declspec(noinline) CMemoryAllocator::~CMemoryAllocator(){CHECK(false);}
__declspec(noinline) void FABLE_FASTCALL CMemoryAllocator::operator delete(void*){CHECK(false);}
// Not yet recreated in this unit; never reached here.
extern "C" void GlyphCold(){fprintf(stderr,"GLYPH_FAILURE cold virtual op=%u\n",op);fflush(stderr);ExitProcess(80);}
#pragma comment(linker,"/alternatename:?Preload@CStreamingFontBank@@UBE_NABVCWideString@@_N@Z=_GlyphCold")
#pragma comment(linker,"/alternatename:?PrepareToRender@CStreamingFontBank@@UBEX_N@Z=_GlyphCold")
#pragma comment(linker,"/alternatename:?Render@CStreamingFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z=_GlyphCold")
// Input-stream virtuals the memory stream's vtable names but the font record read never reaches.
#define FABLE_STUBS_NO_MEMORY_DATA_INPUT_STREAM
#include "test_support/data_input_stream_stubs.h"
void CDataInputStream::SetPosition(unsigned long){CHECK(false);}
// vector<char>::resize is specialised in the module unit; keep an out-of-line copy for the stream provider.
void (_STL::vector<char>::*const KeepCharResize)(size_t,const char&)=&_STL::vector<char>::resize;
// String instance counters and empty-string sentinels (engine globals defined by fixtures).
fable_i32 g_CCharStringInstanceCount_013BD800=0,g_CWideStringInstanceCount_013BCA20=0;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};char g_FableEmptyNarrowString_0129AAF4[1]={0};
CFileDataInputStream::~CFileDataInputStream(){CHECK(false);}
// Text files for ReadTextFileIntoArray: UTF-16LE and UTF-16BE with byte-order marks, and 8-bit.
struct TextFile{const wchar_t* name;unsigned char bytes[96];unsigned long size;};
static TextFile textFiles[3];static const TextFile* openFile;static unsigned fileOpens,fileReads,fileCloses;
__declspec(noinline) CAFile::CAFile(){}
#pragma comment(linker,"/include:??0CAFile@@QAE@XZ")
__declspec(noinline) void CDiskFileWin32::Open(const CWideString& path,FileAccessType access,unsigned long flags){CHECK(access==FILE_ACCESS_READ && flags==2 && !openFile);
 for(unsigned i=0;i<3;++i)if(!wcscmp((const wchar_t*)path.GetAsCharArray(),textFiles[i].name))openFile=&textFiles[i];CHECK(openFile!=0);Length=(long)openFile->size;Opened=true;++fileOpens;}
__declspec(noinline) void CDiskFileWin32::Read(void* d,long n,CASuspendableProcess* p){CHECK(openFile && !p && n==(long)openFile->size);memcpy(d,openFile->bytes,n);++fileReads;}
__declspec(noinline) void CDiskFileWin32::Close(){if(openFile)++fileCloses;openFile=0;Opened=false;}
void CDiskFileWin32::Write(const void*,long,CASuspendableProcess*){CHECK(false);}
void CDiskFileWin32::SetPosition(long){CHECK(false);}
void CDiskFileWin32::MovePosition(long){CHECK(false);}
long CDiskFileWin32::GetPosition()const{CHECK(false);return 0;}
void CDiskFileWin32::SetLength(long){CHECK(false);}
CAFile* CDiskFileWin32::Clone()const{CHECK(false);return 0;}
#pragma comment(linker,"/alternatename:?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ=_GlyphCold")
// The narrow-name ReadTextFileIntoArray (exact-gated separately) is the only ToWideString caller here.
#pragma comment(linker,"/alternatename:?ToWideString@CCharString@@QBE?AVCWideString@@XZ=_GlyphCold")
#pragma comment(linker,"/include:?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z")
#pragma comment(linker,"/include:?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z")
#pragma comment(linker,"/include:?Close@CDiskFileWin32@@UAEXXZ")
template void _STL::vector<unsigned short>::resize(size_t,const unsigned short&);
template void _STL::vector<char>::_M_fill_insert(char*,size_t,const char&);
// vector<unsigned char>::_M_fill_insert (retail 0x00485F3A): the STLport instantiation, shared by both modes.
template void _STL::vector<unsigned char>::_M_fill_insert(unsigned char*,size_t,const unsigned char&);
// Generated data.
static unsigned char* file;static unsigned long fileSize;
static unsigned U(unsigned long& at){CHECK(at+4<=fileSize);unsigned v;memcpy(&v,file+at,4);at+=4;return v;}
__declspec(align(16)) static unsigned char fontStorage[2][sizeof(CStreamingFontBank)];static GlyphBank bankCache[2];
static CStreamingFontBank* fonts[2]={(CStreamingFontBank*)fontStorage[0],(CStreamingFontBank*)fontStorage[1]};
static unsigned char* bankBlobs[2];
static void Load(const char* name){FILE* input=fopen(name,"rb");CHECK(input);fseek(input,0,SEEK_END);fileSize=ftell(input);fseek(input,0,SEEK_SET);file=(unsigned char*)malloc(fileSize);CHECK(fread(file,1,fileSize,input)==fileSize && !fclose(input));
 CHECK(!memcmp(file,"SGLY",4));unsigned long at=4;const unsigned perChunk=U(at),chunks=U(at),adjustments=U(at),blob=U(at),glyphs=U(at);CHECK((glyphs+perChunk-1)/perChunk==chunks);
 const unsigned char* indices=file+at;at+=4*(chunks+1);const unsigned char* data=file+at;at+=blob;const unsigned char* adjust=file+at;at+=2*adjustments;
 streamSize=0x14+blob+4*(chunks+1)+2*adjustments;streamBlob=(unsigned char*)malloc(streamSize);const unsigned long header[5]={glyphs,adjustments,0x5A5A0,blob,perChunk};
 memcpy(streamBlob,header,0x14);memcpy(streamBlob+0x14,data,blob);memcpy(streamBlob+0x14+blob,indices,4*(chunks+1));memcpy(streamBlob+0x14+blob+4*(chunks+1),adjust,2*adjustments);
 for(unsigned f=0;f<2;++f){CStreamingFontBank* font=fonts[f];memset(font,0,sizeof(CStreamingFontBank));font->FontHeight=U(at);font->fontIndex=U(at);memcpy(font->GlyphFlags,file+at,sizeof(font->GlyphFlags));at+=sizeof(font->GlyphFlags);
  unsigned long offsets[1024];memcpy(offsets,file+at,sizeof(offsets));at+=sizeof(offsets);const unsigned size=U(at);bankBlobs[f]=file+at;at+=size;
  for(unsigned b=0;b<1024;++b)font->GlyphBanks[b]=offsets[b]==0xFFFFFFFF?0:bankBlobs[f]+offsets[b];font->glyphBankCache=&bankCache[f];font->glyphBankCacheIndex=0xFFFFFFFF;
  recordSizes[f]=U(at);records[f]=file+at;at+=recordSizes[f];}
 CHECK(at==fileSize);}
// State the three functions may change.
struct State{CStreamingGlyphLookup lookup[256][8];unsigned long chunk;CStreamingGlyphData data[0x40];GlyphBank banks[2];unsigned long bankIndex[2];StreamingFontCacheEntry small[512],big[128];};
static State before,after;
static void Save(State& s){memcpy(s.lookup,glyphLookup,sizeof(s.lookup));s.chunk=glyphTextureCacheGlyphDataCacheChunk;memcpy(s.data,glyphTextureCacheGlyphDataCache,sizeof(s.data));memcpy(s.banks,bankCache,sizeof(s.banks));for(unsigned f=0;f<2;++f)s.bankIndex[f]=fonts[f]->glyphBankCacheIndex;memcpy(s.small,g_StreamingFontCacheBufferData.entries,sizeof(s.small));memcpy(s.big,g_StreamingFontCacheBufferData2.entries,sizeof(s.big));}
static void Restore(const State& s){memcpy(glyphLookup,s.lookup,sizeof(s.lookup));glyphTextureCacheGlyphDataCacheChunk=s.chunk;memcpy(glyphTextureCacheGlyphDataCache,s.data,sizeof(s.data));memcpy(bankCache,s.banks,sizeof(s.banks));for(unsigned f=0;f<2;++f)fonts[f]->glyphBankCacheIndex=s.bankIndex[f];memcpy(g_StreamingFontCacheBufferData.entries,s.small,sizeof(s.small));memcpy(g_StreamingFontCacheBufferData2.entries,s.big,sizeof(s.big));}
static void Same(){State now;Save(now);if(memcmp(&now,&after,sizeof(now))){const unsigned char* a=(const unsigned char*)&after;const unsigned char* b=(const unsigned char*)&now;unsigned k=0;while(a[k]==b[k])++k;fprintf(stderr,"STATE_DIFF op=%u offset=%u candidate=%02x original=%02x\n",op,k,a[k],b[k]);CHECK(false);}}
static void* DataAddress(unsigned index){
 void* table[]={glyphLookup,&g_StreamingFontCacheBufferData,&g_StreamingFontCacheBufferData2,&numTextureCacheGlyphDatasPerChunk,&glyphTextureCacheGlyphDataCacheChunk,
  &glyphTextureCacheGlyphDataIndices,&glyphTextureCacheGlyphData,&glyphTextureCacheGlyphDataCache,&glyphTextureCacheGlyphAdjustment,
  &GFontTextureBytesPerPixel,&textureCacheMem,&textureCacheSize,&glyphTextureCacheGlyphDataSize,&glyphTextureCacheGlyphDataNumIndices,
  &textureCacheNumGlyphs,&textureCacheNumGlyphAdjustments,&textureCacheMemOffset,&GFontTextureCache,&GFontPixelLookupTable,streamingFontBankList,GFontTextureCacheUpdateMask,&GFontTextureCacheUpdateFlag};
 CHECK(index<sizeof(table)/sizeof(table[0]));return table[index];}
static unsigned lookups,hits,nulls,extents;static bool trace;
// Faulting address on an access violation (CONTEXT::Eip at +0xB8; EXCEPTION_RECORD address at +0x0C).
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
static long __stdcall Crash(void** pointers){const unsigned char* record=(const unsigned char*)pointers[0];const unsigned char* context=(const unsigned char*)pointers[1];
 fprintf(stderr,"GLYPH_FAILURE crash op=%u mode=%u eip=%08x data=%08x\n",op,mode,*(const unsigned*)(context+0xB8),*(const unsigned*)(record+0x18));fflush(stderr);ExitProcess(81);return 0;}
static void Trace(const char* what){if(trace){fprintf(stderr,"%s op=%u mode=%u\n",what,op,mode);fflush(stderr);}}
static void Lookup(unsigned f,unsigned short ch){Trace("lookup");Save(before);
 mode=0;CStreamingGlyphLookup* r=(fonts[f]->*Probe::Lookup())(ch);Save(after);
 Restore(before);mode=1;CStreamingGlyphLookup* o=(fonts[f]->*Member<LookupFn>(LOOKUP_AT))(ch);CHECK(r==o);Same();
 ++lookups;if(!r)++nulls;++op;}
static void Extents(unsigned f,const wchar_t* text,bool first){Trace("extents");Save(before);
 mode=0;C2DExtentsF r=fonts[f]->CStreamingFontBank::GetStringExtents(text,first);Save(after);
 Restore(before);mode=1;Trace("extents-retail");C2DExtentsF o=(fonts[f]->*Member<ExtentsFn>(EXTENTS_AT))(text,first);CHECK(!memcmp(&r,&o,sizeof(r)));Same();++extents;++op;}
static void Find(unsigned f,unsigned short ch){Trace("find");Save(before);
 mode=0;const CStreamingGlyph* r=(fonts[f]->*Probe::Find())(ch);Save(after);
 Restore(before);mode=1;const CStreamingGlyph* o=(fonts[f]->*Member<FindFn>(FIND_AT))(ch);CHECK(r==o);Same();++op;}
// Texture-cache initialisation: everything it sets, with its allocations' contents.
struct InitState{long bpp;unsigned char* mem;unsigned long memSize,dataSize,numIndices,numGlyphs,numAdjust,memOffset,perChunk,chunk;size_t lookupSize;unsigned char lookup[1024];
 bool haveArrays;unsigned textureCount,bestCalls,streamCalls,readCalls,allocCalls,createCalls,enterCalls,leaveCalls;TextureCall calls[4];void* allocator;};
static InitState initAfter;static unsigned char* initArrays[2];static unsigned long initArraySize;
static void InitReset(bool allocatorExists){CArray<unsigned char>().swap(GFontPixelLookupTable);GFontTextureBytesPerPixel=2;textureCacheMem=0;textureCacheSize=0;numTextureCacheGlyphDatasPerChunk=0x40;glyphTextureCacheGlyphDataCacheChunk=0xFFFFFFFF;
 textureCacheNumGlyphs=textureCacheNumGlyphAdjustments=textureCacheMemOffset=glyphTextureCacheGlyphDataSize=glyphTextureCacheGlyphDataNumIndices=0;
 delete[] glyphTextureCacheGlyphDataCache;delete[] (unsigned char*)glyphTextureCacheGlyphData;delete[] glyphTextureCacheGlyphDataIndices;delete[] glyphTextureCacheGlyphAdjustment;
 glyphTextureCacheGlyphDataCache=0;glyphTextureCacheGlyphData=0;glyphTextureCacheGlyphDataIndices=0;glyphTextureCacheGlyphAdjustment=0;
 textureCount=bestCalls=streamCalls=readCalls=allocCalls=createCalls=enterCalls=leaveCalls=0;memset(textureCalls,0,sizeof(textureCalls));g_FableMemoryAllocator_013CAA90=allocatorExists?(CMemoryAllocator*)allocatorObject:0;}
static void InitSave(InitState& s,unsigned slot){memset(&s,0,sizeof(s));s.bpp=GFontTextureBytesPerPixel;s.mem=textureCacheMem;s.memSize=textureCacheSize;s.dataSize=glyphTextureCacheGlyphDataSize;s.numIndices=glyphTextureCacheGlyphDataNumIndices;
 s.numGlyphs=textureCacheNumGlyphs;s.numAdjust=textureCacheNumGlyphAdjustments;s.memOffset=textureCacheMemOffset;s.perChunk=numTextureCacheGlyphDatasPerChunk;s.chunk=glyphTextureCacheGlyphDataCacheChunk;
 s.lookupSize=GFontPixelLookupTable.size();CHECK(s.lookupSize<=sizeof(s.lookup));if(s.lookupSize)memcpy(s.lookup,&GFontPixelLookupTable[0],s.lookupSize);
 s.haveArrays=glyphTextureCacheGlyphDataCache!=0;s.textureCount=textureCount;s.bestCalls=bestCalls;s.streamCalls=streamCalls;s.readCalls=readCalls;s.allocCalls=allocCalls;s.createCalls=createCalls;s.enterCalls=enterCalls;s.leaveCalls=leaveCalls;
 memcpy(s.calls,textureCalls,sizeof(s.calls));s.allocator=g_FableMemoryAllocator_013CAA90;
 // Loaded arrays (glyph data, chunk index, adjustments) concatenated for comparison; the chunk cache is uninitialised storage.
 free(initArrays[slot]);initArrays[slot]=0;initArraySize=0;
 if(s.haveArrays){const unsigned long a=s.dataSize,b=(s.numIndices+1)*4,c=s.numAdjust*2;initArraySize=a+b+c;initArrays[slot]=(unsigned char*)malloc(initArraySize);
  memcpy(initArrays[slot],glyphTextureCacheGlyphData,a);memcpy(initArrays[slot]+a,glyphTextureCacheGlyphDataIndices,b);memcpy(initArrays[slot]+a+b,glyphTextureCacheGlyphAdjustment,c);}}
typedef void (FABLE_FASTCALL* InitFn)(CStreamingFontDataBank*);
// Preload: everything it reads or writes, the 512 KiB pixel cache included.
__declspec(align(16)) static unsigned char fontManagerStorage[sizeof(CFontManager)];static CFontManager* const fontManager=(CFontManager*)fontManagerStorage;
struct CFontManagerTestAccess{static unsigned long& Pending(){return fontManager->NumberAsyncGlyphsPending;}static bool& Asynchronous(){return fontManager->LoadFontGlyphsAsynchronously;}};
struct PreloadState{unsigned char ring[sizeof(PendingLoadedGlyph)*512];unsigned long head,tail,pending;bool asynchronous,flag;unsigned char mask[512];CStreamingGlyphLookup lookup[256][8];
 StreamingFontCacheEntry small[512],big[128];unsigned long smallNewest,smallOldest,bigNewest,bigOldest;GlyphBank banks[2];unsigned long bankIndex[2],chunk;CStreamingGlyphData data[0x40];
 unsigned asyncUsed,pixelReads;bool finished[ASYNC_POOL],deleted[ASYNC_POOL];unsigned long refs[ASYNC_POOL];};
static PreloadState preloadBefore,preloadAfter;static unsigned char* pixelsBefore;static unsigned char* pixelsAfter;
static void PreloadSave(PreloadState& s){memset(&s,0,sizeof(s));memcpy(s.ring,g_PendingLoadedGlyphs,sizeof(s.ring));s.head=g_HeadPendingLoadedGlyphs;s.tail=g_TailPendingLoadedGlyphs;s.pending=CFontManagerTestAccess::Pending();s.asynchronous=CFontManagerTestAccess::Asynchronous();s.flag=GFontTextureCacheUpdateFlag;
 memcpy(s.mask,GFontTextureCacheUpdateMask,sizeof(s.mask));memcpy(s.lookup,glyphLookup,sizeof(s.lookup));memcpy(s.small,g_StreamingFontCacheBufferData.entries,sizeof(s.small));memcpy(s.big,g_StreamingFontCacheBufferData2.entries,sizeof(s.big));
 s.smallNewest=g_StreamingFontCacheBufferData.newestEntry;s.smallOldest=g_StreamingFontCacheBufferData.oldestEntry;s.bigNewest=g_StreamingFontCacheBufferData2.newestEntry;s.bigOldest=g_StreamingFontCacheBufferData2.oldestEntry;
 for(unsigned f=0;f<2;++f){memcpy(&s.banks[f],fonts[f]->glyphBankCache,sizeof(GlyphBank));s.bankIndex[f]=fonts[f]->glyphBankCacheIndex;}s.chunk=glyphTextureCacheGlyphDataCacheChunk;memcpy(s.data,glyphTextureCacheGlyphDataCache,sizeof(s.data));
 s.asyncUsed=asyncUsed;s.pixelReads=pixelReads;memcpy(s.finished,asyncFinished,sizeof(s.finished));memcpy(s.deleted,asyncDeleted,sizeof(s.deleted));for(unsigned i=0;i<ASYNC_POOL;++i)s.refs[i]=asyncInfos[i].RefCount;}
static void PreloadRestore(const PreloadState& s){memcpy(g_PendingLoadedGlyphs,s.ring,sizeof(s.ring));g_HeadPendingLoadedGlyphs=s.head;g_TailPendingLoadedGlyphs=s.tail;CFontManagerTestAccess::Pending()=s.pending;CFontManagerTestAccess::Asynchronous()=s.asynchronous;GFontTextureCacheUpdateFlag=s.flag;
 memcpy(GFontTextureCacheUpdateMask,s.mask,sizeof(s.mask));memcpy(glyphLookup,s.lookup,sizeof(s.lookup));memcpy(g_StreamingFontCacheBufferData.entries,s.small,sizeof(s.small));memcpy(g_StreamingFontCacheBufferData2.entries,s.big,sizeof(s.big));
 g_StreamingFontCacheBufferData.newestEntry=s.smallNewest;g_StreamingFontCacheBufferData.oldestEntry=s.smallOldest;g_StreamingFontCacheBufferData2.newestEntry=s.bigNewest;g_StreamingFontCacheBufferData2.oldestEntry=s.bigOldest;
 for(unsigned f=0;f<2;++f){memcpy(fonts[f]->glyphBankCache,&s.banks[f],sizeof(GlyphBank));fonts[f]->glyphBankCacheIndex=s.bankIndex[f];}glyphTextureCacheGlyphDataCacheChunk=s.chunk;memcpy(glyphTextureCacheGlyphDataCache,s.data,sizeof(s.data));
 asyncUsed=s.asyncUsed;pixelReads=s.pixelReads;memcpy(asyncFinished,s.finished,sizeof(s.finished));memcpy(asyncDeleted,s.deleted,sizeof(s.deleted));for(unsigned i=0;i<ASYNC_POOL;++i)asyncInfos[i].RefCount=s.refs[i];}
typedef bool (CStreamingFontBank::*PreloadFn)(const CWideString&,bool) const;
// PrepareToRender: the streaming font's 23 render states through the inline setters, from seeded managers.
__declspec(align(16)) static unsigned char renderStorage[sizeof(CRenderManager)];static CRenderManager* const render=(CRenderManager*)renderStorage;
static unsigned char renderBefore[sizeof(CRenderManager)],renderAfter[sizeof(CRenderManager)];
static const unsigned prepareOffsets[23]={0x28b8,0x291c,0x2930,0x28cc,0x28e0,0x28a4,0x282c,0x2890,0x2868,0x2944,0x2a20,0x30c4,0x3164,0x2e44,0x3664,0x3204,0x3344,0x32a4,0x3484,0x3524,0x3218,0x32b8,0x35c4};
struct CRenderStateManagerTestAccess{static void Counts(unsigned stack,unsigned dirty,unsigned mask){render->StateStackSize=stack;render->StateUpdateListSize=dirty;render->CurrentBookmarkMask=mask;}};
typedef void (CStreamingFontBank::*PrepareFn)(bool) const;
static unsigned prepares;
static void Prepare(unsigned seed,bool stages,unsigned mask,unsigned capacity){Trace("prepare");
 memset(renderStorage,0xA5,sizeof(renderStorage));CRenderStateManagerTestAccess::Counts(capacity==0?0:capacity==1?51:998,capacity==0?0:capacity==1?13:230,mask);
 for(unsigned i=0;i<23;++i){CRenderStateManager::CStateInfo* info=(CRenderStateManager::CStateInfo*)(renderStorage+prepareOffsets[i]);info->DesiredState=((seed+i)%3)==0?(seed&1?1:0):0x11111111*((seed+i)%7);
  info->BookmarkMask=((seed+i)%4)==0?mask:((seed+i)%4)==1?~mask:0;info->DirtyListFlag=((seed+i)&1)!=0;}
 memcpy(renderBefore,renderStorage,sizeof(renderBefore));
 mode=0;fonts[seed&1]->CStreamingFontBank::PrepareToRender(stages);memcpy(renderAfter,renderStorage,sizeof(renderAfter));
 memcpy(renderStorage,renderBefore,sizeof(renderBefore));mode=1;(fonts[seed&1]->*Member<PrepareFn>(PREPARE_AT))(stages);
 if(memcmp(renderAfter,renderStorage,sizeof(renderAfter))){unsigned k=0;while(renderAfter[k]==renderStorage[k])++k;fprintf(stderr,"RENDER_DIFF op=%u offset=%x candidate=%02x original=%02x\n",op,k,renderAfter[k],renderStorage[k]);CHECK(false);}
 ++prepares;++op;}
// ReadTextFileIntoArray over the three files.
typedef unsigned long (FABLE_FASTCALL* ReadTextFn)(const CWideString&,CArray<unsigned short>&);
static unsigned textReads;
static void ReadText(unsigned f){Trace("read-text");CWideString name(textFiles[f].name);CArray<unsigned short> a,b;a.resize(64,0xBEEF);b.resize(64,0xBEEF);
 mode=0;const unsigned long r=ReadTextFileIntoArray(name,a);CHECK(!openFile);mode=1;const unsigned long o=((ReadTextFn)(code+READ_TEXT_AT))(name,b);CHECK(!openFile);
 CHECK(r==o && a.size()==b.size() && a.size()==r+1 && !memcmp(&a[0],&b[0],a.size()*2) && !a[r]);++textReads;++op;}
// Texture-cache update: changed cells converted through the lookup table for 1, 2 and 4 byte texels.
typedef void (FABLE_FASTCALL* UpdateFn)();
static unsigned textureUpdates;static unsigned char* texelsAfter;static unsigned char updateMaskAfter[512];
// Render: device SetTexture, SetVertexFormat, DrawAsIndexedTris (vertices and indices by value) and the render
// target query are logged per side; the logs, the whole render manager, the glyph cache and texels must agree.
static unsigned char renderLog[2][0x30000];static unsigned renderLogSize[2],renderSide;
static void LogBytes(const void* p,unsigned n){CHECK(renderLogSize[renderSide]+n<=sizeof(renderLog[0]));memcpy(renderLog[renderSide]+renderLogSize[renderSide],p,n);renderLogSize[renderSide]+=n;}
static void LogWord(unsigned v){LogBytes(&v,4);}
static void* renderDeviceTable[119];static void* renderDeviceObject[1]={renderDeviceTable};
static HRESULT __stdcall DeviceSetTexture(void* self,unsigned long stage,void* texture){CHECK(self==renderDeviceObject);LogWord(1);LogWord(stage);LogWord((unsigned)texture);return 0;}
static long targetX,targetY;static unsigned renderDraws,renderFormats;
__declspec(noinline) void CRenderManagerCore::SetVertexFormat(unsigned long format){LogWord(2);LogWord(format);++renderFormats;}
__declspec(noinline) void CRenderManagerCore::DrawAsIndexedTris(const void* vertices,long stride,long count,void* indices,long primitives){
 CHECK(count>0 && count<=256 && primitives*2==count);LogWord(3);LogWord(stride);LogWord(count);LogWord(primitives);LogBytes(vertices,count*stride);LogBytes(indices,primitives*6);++renderDraws;}
__declspec(noinline) const C2DExtentsI CDisplayManager::GetRenderTargetDimensions(){CHECK(this==display);LogWord(4);return C2DExtentsI(targetX,targetY);}
#pragma comment(linker,"/include:?SetVertexFormat@CRenderManagerCore@@QAEXK@Z")
#pragma comment(linker,"/include:?DrawAsIndexedTris@CRenderManagerCore@@QAEXPBXJJPAXJ@Z")
#pragma comment(linker,"/include:?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ")
static void UpdateTexture(unsigned bytes,unsigned seed){Trace("texture-update");
 GFontTextureBytesPerPixel=bytes;GFontPixelLookupTable.resize(256*bytes);for(unsigned i=0;i<256*bytes;++i)GFontPixelLookupTable[i]=(unsigned char)(i*37+seed);
 for(unsigned c=0;c<512;++c){seed=seed*1103515245+12345;GFontTextureCacheUpdateMask[c]=(unsigned char)((seed>>20)%3==0);}
 for(unsigned long i=0;i<0x80000;++i)textureMemory[i]=(unsigned char)((i>>3)*13+i+seed);GFontTextureCacheUpdateFlag=true;
 unsigned char mask[512];memcpy(mask,GFontTextureCacheUpdateMask,sizeof(mask));memset(texelMemory,0xCD,TEXEL_PITCH*512);
 mode=0;GFUpdateFontTextureCache();CHECK(!textureLocked && !GFontTextureCacheUpdateFlag);memcpy(texelsAfter,texelMemory,TEXEL_PITCH*512);memcpy(updateMaskAfter,GFontTextureCacheUpdateMask,sizeof(updateMaskAfter));
 memcpy(GFontTextureCacheUpdateMask,mask,sizeof(mask));memset(texelMemory,0xCD,TEXEL_PITCH*512);GFontTextureCacheUpdateFlag=true;
 mode=1;((UpdateFn)(code+TEXTURE_UPDATE_AT))();CHECK(!textureLocked && !GFontTextureCacheUpdateFlag);
 CHECK(!memcmp(updateMaskAfter,GFontTextureCacheUpdateMask,sizeof(updateMaskAfter)));for(unsigned c=0;c<512;++c)CHECK(!GFontTextureCacheUpdateMask[c]);
 if(memcmp(texelsAfter,texelMemory,TEXEL_PITCH*512)){unsigned k=0;while(texelsAfter[k]==texelMemory[k])++k;fprintf(stderr,"TEXEL_DIFF op=%u offset=%u candidate=%02x original=%02x\n",op,k,texelsAfter[k],texelMemory[k]);CHECK(false);}
 ++textureUpdates;++op;}
static unsigned preloads,preloadMisses;
typedef void (CStreamingFontBank::*RenderFn)(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*) const;
static unsigned renders,renderSkips;static unsigned char* texelsBefore;static unsigned char* texelsRendered;
static void RenderText(unsigned f,const wchar_t* text,float x,float y,float z,float scale,unsigned long colourValue,unsigned long flags,const CTexture* texture,unsigned seed){Trace("render");
 CWideString wide(text);const CRGBColour& colour=*reinterpret_cast<const CRGBColour*>(&colourValue);const C3DVector position(x,y,z);
 FixtureShaderManager[0x390]=(seed&1)!=0;FixtureShaderManager[0x391]=(seed&2)!=0;CFontManagerTestAccess::Asynchronous()=(seed%5)==0;inFlight=(seed%3)==0;
 memset(renderStorage,0,sizeof(renderStorage));render->PD3DDevice=(IDirect3DDevice9*)renderDeviceObject;renderDeviceTable[65]=(void*)&DeviceSetTexture;
 render->TextureStagesInUse=(long)(seed%4);render->CurrentlySelectedTextures[0]=(seed&4)?GFontTextureCache.PD3DTexture:(IDirect3DBaseTexture9*)0x1234;
 render->CurrentlySelectedTextures[1]=(seed&8)?(IDirect3DBaseTexture9*)0:texture?texture->PD3DTexture:(IDirect3DBaseTexture9*)0x5678;
 const unsigned masks[]={0,1,0x80000000,0xffffffff};CRenderStateManagerTestAccess::Counts((seed>>4)%3==0?0:51,(seed>>4)%3==0?0:13,masks[(seed>>6)%4]);
 const unsigned stateOffsets[7]={0x3218,0x3358,0x32b8,0x3498,0x3538,0x322c,0x32cc};
 for(unsigned i=0;i<7;++i){CRenderStateManager::CStateInfo* info=(CRenderStateManager::CStateInfo*)(renderStorage+stateOffsets[i]);info->DesiredState=((seed+i)%3)==0?1:0x01010101*((seed+i)%5);info->BookmarkMask=((seed+i)%4)==0?masks[(seed>>6)%4]:0;info->DirtyListFlag=((seed+i)&1)!=0;}
 const long targets[3][2]={{640,480},{1280,720},{1023,767}};targetX=targets[seed%3][0];targetY=targets[seed%3][1];
 memcpy(renderBefore,renderStorage,sizeof(renderBefore));PreloadSave(preloadBefore);memcpy(pixelsBefore,textureMemory,0x80000);memcpy(texelsBefore,texelMemory,TEXEL_PITCH*512);
 mode=0;renderSide=0;renderLogSize[0]=0;fonts[f]->CStreamingFontBank::Render(wide,position,scale,colour,flags,texture);CHECK(!textureLocked);
 memcpy(renderAfter,renderStorage,sizeof(renderAfter));PreloadSave(preloadAfter);memcpy(pixelsAfter,textureMemory,0x80000);memcpy(texelsRendered,texelMemory,TEXEL_PITCH*512);
 memcpy(renderStorage,renderBefore,sizeof(renderBefore));PreloadRestore(preloadBefore);memcpy(textureMemory,pixelsBefore,0x80000);memcpy(texelMemory,texelsBefore,TEXEL_PITCH*512);
 mode=1;renderSide=1;renderLogSize[1]=0;(fonts[f]->*Member<RenderFn>(RENDER_AT))(wide,position,scale,colour,flags,texture);CHECK(!textureLocked);
 if(renderLogSize[0]!=renderLogSize[1] || memcmp(renderLog[0],renderLog[1],renderLogSize[0])){unsigned k=0;while(k<renderLogSize[0] && k<renderLogSize[1] && renderLog[0][k]==renderLog[1][k])++k;fprintf(stderr,"RENDER_LOG_DIFF op=%u sizes=%u/%u offset=%u\n",op,renderLogSize[0],renderLogSize[1],k);CHECK(false);}
 if(memcmp(renderAfter,renderStorage,sizeof(renderAfter))){unsigned k=0;while(renderAfter[k]==renderStorage[k])++k;fprintf(stderr,"RENDER_DIFF op=%u offset=%x candidate=%02x original=%02x\n",op,k,renderAfter[k],renderStorage[k]);CHECK(false);}
 PreloadState now;PreloadSave(now);CHECK(!memcmp(&now,&preloadAfter,sizeof(now)));CHECK(!memcmp(pixelsAfter,textureMemory,0x80000));CHECK(!memcmp(texelsRendered,texelMemory,TEXEL_PITCH*512));
 if(!renderLogSize[0])++renderSkips;++renders;++op;}
static void Preload(unsigned f,const wchar_t* text,bool asynchronous,bool flight){Trace("preload");inFlight=flight;CWideString wide(text);
 PreloadSave(preloadBefore);memcpy(pixelsBefore,textureMemory,0x80000);
 mode=0;const bool r=fonts[f]->CStreamingFontBank::Preload(wide,asynchronous);PreloadSave(preloadAfter);memcpy(pixelsAfter,textureMemory,0x80000);
 PreloadRestore(preloadBefore);memcpy(textureMemory,pixelsBefore,0x80000);mode=1;const bool o=(fonts[f]->*Member<PreloadFn>(PRELOAD_AT))(wide,asynchronous);
 CHECK(r==o);PreloadState now;PreloadSave(now);
 if(memcmp(&now,&preloadAfter,sizeof(now))){const unsigned char* a=(const unsigned char*)&preloadAfter;const unsigned char* b=(const unsigned char*)&now;unsigned k=0;while(a[k]==b[k])++k;fprintf(stderr,"PRELOAD_DIFF op=%u offset=%u candidate=%02x original=%02x\n",op,k,a[k],b[k]);CHECK(false);}
 CHECK(!memcmp(pixelsAfter,textureMemory,0x80000));++preloads;if(!r)++preloadMisses;++op;}
// Constructor and destructor (thiscall, callee-cleaned) called through fastcall pointers (ecx = this).
typedef void* (FABLE_FASTCALL* CtorFn)(void*,void*,CStreamingFontDataBank*,unsigned long);
typedef void (FABLE_FASTCALL* DtorFn)(void*,void*);
__declspec(align(16)) static unsigned char builtStorage[2][2][sizeof(CStreamingFontBank)];
static unsigned constructions,destructions;
// Compare two constructed fonts: owned allocations by content (bank memory, bank cache, texture), the name by text,
// GlyphBanks as offsets into the bank memory, every other byte exactly.
static void SameFont(const CStreamingFontBank& a,const CStreamingFontBank& b,unsigned long bankBytes){
 CHECK(!strcmp(a.Name.GetConstCharPtr(),b.Name.GetConstCharPtr()));
 CHECK(!memcmp(a.glyphBankCache,b.glyphBankCache,sizeof(GlyphBank)) && !memcmp(a.GlyphBankMemory,b.GlyphBankMemory,bankBytes) && !memcmp(a.PTexture,b.PTexture,sizeof(CTexture)));
 for(unsigned i=0;i<1024;++i){CHECK(!a.GlyphBanks[i]==!b.GlyphBanks[i]);if(a.GlyphBanks[i])CHECK((unsigned char*)a.GlyphBanks[i]-(unsigned char*)a.GlyphBankMemory==(unsigned char*)b.GlyphBanks[i]-(unsigned char*)b.GlyphBankMemory);}
 unsigned char x[sizeof(CStreamingFontBank)],y[sizeof(CStreamingFontBank)];memcpy(x,&a,sizeof(x));memcpy(y,&b,sizeof(y));
 const unsigned skip[][2]={{8,4},{0x14,4},{0x28,4},{0x2c,0x1000},{0x3030,4}};
 for(unsigned s=0;s<5;++s){memset(x+skip[s][0],0,skip[s][1]);memset(y+skip[s][0],0,skip[s][1]);}
 if(memcmp(x,y,sizeof(x))){unsigned k=0;while(x[k]==y[k])++k;fprintf(stderr,"FONT_DIFF op=%u offset=%x candidate=%02x original=%02x\n",op,k,x[k],y[k]);CHECK(false);}}
static unsigned long BankBytes(unsigned f){unsigned long n;memcpy(&n,records[f]+strlen((const char*)records[f])+1+13+1024*4,4);return n;}
static void FreshEntries(){StreamingFontCacheBufferData& s=g_StreamingFontCacheBufferData;StreamingFontCacheBufferData& g=g_StreamingFontCacheBufferData2;s.entries=g_StreamingFontCacheBufferData.entries;s.numEntries=512;g.entries=g_StreamingFontCacheBufferData2.entries;g.numEntries=128;s.newestEntry=s.oldestEntry=0;g.newestEntry=g.oldestEntry=0;memset(g_StreamingFontCacheBufferData.entries,0xEE,sizeof(g_StreamingFontCacheBufferData.entries));memset(g_StreamingFontCacheBufferData2.entries,0xEE,sizeof(g_StreamingFontCacheBufferData2.entries));}
// Build font f in both modes from the same state (list slots, fresh cache-entry chains). Leaves the list naming
// the recreated font and returns it.
static CStreamingFontBank* Construct(unsigned f,long frame){Trace("ctor");display->FrameCounter=frame;
 CStreamingFontBank* saved[8];memcpy(saved,streamingFontBankList,sizeof(saved));
 unsigned char* a=builtStorage[f][0];unsigned char* b=builtStorage[f][1];memset(a,0xCD,sizeof(builtStorage[f][0]));memset(b,0xCD,sizeof(builtStorage[f][1]));
 FreshEntries();mode=0;Trace("ctor-recreated");CStreamingFontBank* made=new(a) CStreamingFontBank(streamBank,f+1);CHECK(made==(CStreamingFontBank*)a);
 StreamingFontCacheEntry small[512],big[128];memcpy(small,g_StreamingFontCacheBufferData.entries,sizeof(small));memcpy(big,g_StreamingFontCacheBufferData2.entries,sizeof(big));
 CStreamingFontBank* listA[8];memcpy(listA,streamingFontBankList,sizeof(listA));
 memcpy(streamingFontBankList,saved,sizeof(saved));FreshEntries();mode=1;Trace("ctor-retail");CHECK(((CtorFn)(code+CTOR_AT))(b,0,streamBank,f+1)==b);
 CHECK(!memcmp(small,g_StreamingFontCacheBufferData.entries,sizeof(small)) && !memcmp(big,g_StreamingFontCacheBufferData2.entries,sizeof(big)));
 for(unsigned i=0;i<8;++i)CHECK(listA[i]==(streamingFontBankList[i]==(CStreamingFontBank*)b?(CStreamingFontBank*)a:streamingFontBankList[i]));
 SameFont(*(CStreamingFontBank*)a,*(CStreamingFontBank*)b,BankBytes(f));
 memcpy(streamingFontBankList,listA,sizeof(listA));++constructions;++op;return made;}
// Destroy both builds of font f from the same glyph-lookup state; compare the list and lookup effects.
static void Destroy(unsigned f){Trace("dtor");unsigned char* a=builtStorage[f][0];unsigned char* b=builtStorage[f][1];
 static CStreamingGlyphLookup lookupBefore[256][8],lookupAfter[256][8];memcpy(lookupBefore,glyphLookup,sizeof(lookupBefore));
 CStreamingFontBank* saved[8];memcpy(saved,streamingFontBankList,sizeof(saved));
 mode=0;((CStreamingFontBank*)a)->~CStreamingFontBank();memcpy(lookupAfter,glyphLookup,sizeof(lookupAfter));CStreamingFontBank* listA[8];memcpy(listA,streamingFontBankList,sizeof(listA));
 const unsigned long slot=((CStreamingFontBank*)b)->fontIndex;
 memcpy(glyphLookup,lookupBefore,sizeof(lookupBefore));memcpy(streamingFontBankList,saved,sizeof(saved));if(saved[slot]==(CStreamingFontBank*)a)streamingFontBankList[slot]=(CStreamingFontBank*)b;
 mode=1;((DtorFn)(code+DTOR_AT))(b,0);
 CHECK(!memcmp(lookupAfter,glyphLookup,sizeof(lookupAfter)));for(unsigned i=0;i<8;++i)CHECK(listA[i]==streamingFontBankList[i]);
 ++destructions;++op;}
static unsigned inits;
// One initialisation in both modes from the same reset state; leaves the retail mode's result in place.
static void Init(unsigned script,long best,bool allocatorExists,bool twice){Trace("init");textureScript=script;bestIndex=best;
 InitReset(allocatorExists);mode=0;InitializeStreamingFontTextureCache(streamBank);if(twice)InitializeStreamingFontTextureCache(streamBank);InitSave(initAfter,0);
 InitReset(allocatorExists);mode=1;((InitFn)(code+INIT_AT))(streamBank);if(twice)((InitFn)(code+INIT_AT))(streamBank);InitState now;InitSave(now,1);
 if(memcmp(&now,&initAfter,sizeof(now))){const unsigned char* a=(const unsigned char*)&initAfter;const unsigned char* b=(const unsigned char*)&now;unsigned k=0;while(a[k]==b[k])++k;fprintf(stderr,"INIT_DIFF op=%u offset=%u candidate=%02x original=%02x\n",op,k,a[k],b[k]);CHECK(false);}
 CHECK(!initArraySize || !memcmp(initArrays[0],initArrays[1],initArraySize));
 // The data must be the bank's: header values and the three arrays straight from the stream.
 CHECK(now.mem==textureMemory && now.memSize==0x80000 && now.numGlyphs==*(unsigned long*)streamBlob && now.memOffset==0x5A5A0 && now.chunk==0xFFFFFFFF);
 CHECK(initArraySize==streamSize-0x14 && !memcmp(initArrays[1],streamBlob+0x14,initArraySize));
 ++inits;++op;}
int main(int argc,char** argv){CHECK(argc==4);trace=getenv("GLYPH_TRACE")!=0;SetUnhandledExceptionFilter((void*)&Crash);Trace("start");Original(argv[2],argv[3]);Load(argv[1]);Trace("loaded");
 memset(systemStorage,0,sizeof(systemStorage));memset(displayStorage,0,sizeof(displayStorage));fixtureSystem->PDisplayManager.Data=display;fixtureSystem->PFontManager.Data=fontManager;{CRenderManager* pointer=render;memcpy(&display->PRenderManager,&pointer,4);}
 textureMemory=(unsigned char*)malloc(0x80000);texelMemory=(unsigned char*)malloc(TEXEL_PITCH*512);texelsAfter=(unsigned char*)malloc(TEXEL_PITCH*512);pixelsBefore=(unsigned char*)malloc(0x80000);pixelsAfter=(unsigned char*)malloc(0x80000);
 // Texture cache: A8 at once; A8 refused then A4R4G4B4; both refused then the display's best alpha format
 // (A8R8G8B8, A1R5G5B5, A4R4G4B4); allocator created or present; a second call is a no-op.
 const long bests[]={1,5,6};
 for(unsigned exists=0;exists<2;++exists)for(unsigned twice=0;twice<2;++twice){Init(1,1,exists!=0,twice!=0);Init(2,1,exists!=0,twice!=0);for(unsigned b=0;b<3;++b){Init(0,bests[b],exists!=0,twice!=0);Init(4,bests[b],exists!=0,twice!=0);}}
 Init(1,1,true,false);CHECK(GFontTextureBytesPerPixel==1 && GFontPixelLookupTable.size()==256);
 // Fonts from their bank records through the actual constructor (and the retail one, compared).
 for(unsigned f=0;f<2;++f)fonts[f]=Construct(f,-3+(long)f);
 CHECK(fonts[0]->fontIndex==0 && fonts[1]->fontIndex==1 && streamingFontBankList[0]==fonts[0] && streamingFontBankList[1]==fonts[1]);
 StreamingFontCacheBufferData& small=g_StreamingFontCacheBufferData;StreamingFontCacheBufferData& big=g_StreamingFontCacheBufferData2;
 small.entries=g_StreamingFontCacheBufferData.entries;small.numEntries=512;big.entries=g_StreamingFontCacheBufferData2.entries;big.numEntries=128;
 const unsigned short probes[]={0x20,0x41,0x61,0x7e,0xa0,0xe9,0x152,0x3a9,0x25a1,0x3000,0x4e2d,0x4e00,0x141,0x241,0x341,0x441,0x541,0x641,0x741,0x841,0x941,0xa41,0xb41,0x162,0x262,0x362,0x462,0x562,0x0,0xffff,0x7f,0x1f};
 const unsigned np=sizeof(probes)/sizeof(probes[0]);
 // FindGlyph directly: raw/compressed banks, bank-cache reuse and misses.
 for(unsigned f=0;f<2;++f)for(unsigned i=0;i<np;++i){Find(f,probes[i]);Find(f,probes[(i*7+3)%np]);}
 // GlyphLookup over many frames: hits, free ways, eviction by staleness and by age, fallback glyph, chunk switches.
 unsigned seed=12345;
 for(unsigned round=0;round<400;++round){display->FrameCounter=(long)(round/3)-7;
  for(unsigned k=0;k<12;++k){seed=seed*1103515245+12345;const unsigned f=(seed>>16)&1;const unsigned short ch=probes[(seed>>8)%np];Lookup(f,ch);}
  // Every few rounds re-point some ways at cache slots and age or invalidate those slots, as rendering would.
  if(round%5==4){for(unsigned b=0;b<256;b+=(seed>>24)%7+1)for(unsigned w=0;w<8;++w){CStreamingGlyphLookup& way=glyphLookup[b][w];if(!way.ch)continue;seed=seed*1103515245+12345;
   StreamingFontCacheBufferData& cache=way.data.BigChar?big:small;const unsigned pos=(seed>>9)%cache.numEntries;way.cachePos=pos;StreamingFontCacheEntry& slot=cache.entries[pos];
   slot.glyphIndex=(seed>>20)%4==0?way.indices.GlyphIndex+1:way.indices.GlyphIndex;slot.lastUseTime=display->FrameCounter-(long)((seed>>13)%9);if((seed>>27)%11==0)slot.lastUseTime=display->FrameCounter+5;}}}
 // GetStringExtents: lines, first-line-only, no-break space, missing glyphs, bucket collisions, big advances.
 wchar_t texts[8][48];const wchar_t* sources[8]={L"Hello world",L"A\nBB\n\nCCC",L"\x00a0\x00e9\x0152\x03a9 x",L"\x3000\x4e2d\x4e00 tail",L"\x0141\x0241\x0341\x0441\x0541\x0641\x0741\x0841\x0941\x0a41",L"",L"\n",L"~}|{zyx\n\x0162\x0262\x0362"};
 for(unsigned t=0;t<8;++t)wcscpy(texts[t],sources[t]);
 for(unsigned round=0;round<60;++round){display->FrameCounter=1000+round;for(unsigned t=0;t<8;++t)for(unsigned f=0;f<2;++f){Extents(f,texts[t],false);Extents(f,texts[t],true);}}
 for(unsigned b=0;b<256;++b)for(unsigned w=0;w<8;++w)if(glyphLookup[b][w].ch)++hits;
 // Preload: resident and missing glyphs, both cell sizes (cells evicted and overlapping cells invalidated),
 // reads completing at once or left in flight and retired on later calls, synchronous and asynchronous.
 const wchar_t* preloadTexts[]={L"Hello world",L"\x00a0\x00e9\x0152\x03a9 x\n",L"\x0141\x0241\x0341\x0441\x0541\x0641\x0741\x0841\x0941\x0a41",L"\x4e2d\x4e00 tail",L"~}|{zyxwvutsrqponmlkjihgfedcba",L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",L""};
 for(unsigned round=0;round<40;++round){display->FrameCounter=2000+round/2;
  for(unsigned t=0;t<7;++t)Preload((round+t)&1,preloadTexts[t],((round>>1)&1)!=0,((round+t)%3)!=0);
  // Finish some reads in flight; on alternate rounds the texture update has consumed the changed-cell marks.
  for(unsigned k=0;k<asyncUsed;++k)if(!asyncFinished[k] && (k*7+round)%3==0)asyncFinished[k]=true;
  if(round&1){memset(GFontTextureCacheUpdateMask,0,sizeof(GFontTextureCacheUpdateMask));GFontTextureCacheUpdateFlag=false;}}
 for(unsigned k=0;k<asyncUsed;++k)asyncFinished[k]=true;Preload(0,L"A",false,false);CHECK(g_HeadPendingLoadedGlyphs==g_TailPendingLoadedGlyphs && !CFontManagerTestAccess::Pending());
 const unsigned texelBytes[]={1,2,4};for(unsigned s=0;s<6;++s)UpdateTexture(texelBytes[s%3],0x51ED+s*977);
 {const unsigned masks[]={0,1,2,0x80000000,0x10001,0xffffffff};for(unsigned seed=0;seed<16;++seed)for(unsigned stages=0;stages<2;++stages)for(unsigned m=0;m<6;++m)for(unsigned capacity=0;capacity<3;++capacity)Prepare(seed,stages!=0,masks[m],capacity);}
 // Render: lines, alignment flags 2/4/8, spaces and no-break spaces, missing glyphs, big glyphs, a batch flush,
 // mask texture or none, shader paths, resident or not (asynchronous), pending texture updates.
 {texelsBefore=(unsigned char*)malloc(TEXEL_PITCH*512);texelsRendered=(unsigned char*)malloc(TEXEL_PITCH*512);
  CStreamingFontBank::SetAlphaMaskUVs(C2DVector(0.5f,-0.375f),C2DVector(0.5f,0.625f));static unsigned char maskStorage[sizeof(CTexture)];CTexture* mask=(CTexture*)maskStorage;mask->PD3DTexture=(IDirect3DTexture9*)0x7E570000;GFontTextureCache.PD3DTexture=(IDirect3DTexture9*)0x7F000000;
  const wchar_t* renderTexts[]={L"Hello world",L"A\nBB\n\nCCC",L"\x00a0\x00e9\x0152\x03a9 x",L"\x3000\x4e2d\x4e00 tail\n\x4e2d",L"\x0141\x0241\x0341\x0441 \x0541",L"",L"\n",
   L"The quick brown fox jumps over the lazy dog. THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG! 0123456789~}|{zyx"};
  const float scales[]={1.0f,1.5f,0.75f,2.0f};const float xs[]={100.25f,-3.0f,640.0f,17.5f},ys[]={50.5f,7.75f,360.0f,-12.25f};
  unsigned seed=777;
  for(unsigned t=0;t<8;++t)for(unsigned flags=0;flags<16;flags+=2)for(unsigned k=0;k<4;++k){seed=seed*1103515245+12345;const unsigned s=seed>>8;display->FrameCounter=3000+(long)(s%7);
   if(k==3){for(unsigned c=0;c<512;++c)GFontTextureCacheUpdateMask[c]=(unsigned char)((c*31+s)%5==0);GFontTextureCacheUpdateFlag=true;}
   RenderText((s>>3)&1,renderTexts[t],xs[(s>>5)%4],ys[(s>>7)%4],0.125f*((s>>9)%8),scales[(s>>11)%4],0x80FF0000u^(s*2654435761u),flags|((s>>13)&1),(k&1)?mask:0,s);}}
 {const wchar_t sample[]=L"Fable \x00e9\x4e2d\n";const unsigned n=sizeof(sample)/2-1;textFiles[0].name=L"le.txt";textFiles[1].name=L"be.txt";textFiles[2].name=L"ansi.txt";
  textFiles[0].bytes[0]=0xFF;textFiles[0].bytes[1]=0xFE;textFiles[1].bytes[0]=0xFE;textFiles[1].bytes[1]=0xFF;
  for(unsigned i=0;i<n;++i){textFiles[0].bytes[2+2*i]=(unsigned char)sample[i];textFiles[0].bytes[3+2*i]=(unsigned char)(sample[i]>>8);textFiles[1].bytes[2+2*i]=(unsigned char)(sample[i]>>8);textFiles[1].bytes[3+2*i]=(unsigned char)sample[i];}
  textFiles[0].size=textFiles[1].size=2+2*n;const char ansi[]="Lost \xe9 Chapters\r\n";textFiles[2].size=sizeof(ansi)-1;memcpy(textFiles[2].bytes,ansi,textFiles[2].size);
  for(unsigned f=0;f<3;++f)ReadText(f);CHECK(fileOpens==6 && fileReads==6 && fileCloses==6);}
 for(unsigned f=0;f<2;++f)Destroy(f);
 for(unsigned b=0;b<256;++b)for(unsigned w=0;w<8;++w)CHECK(!glyphLookup[b][w].ch);
 UninitializeStreamingFontTextureCache();CHECK(!textureCacheMem && deallocCalls==1 && !glyphTextureCacheGlyphData && !glyphTextureCacheGlyphDataCache);
 printf("STREAMING_GLYPHS_PASS inits=%u fonts=%u/%u preloads=%u/%u reads=%u/%u updates=%u texts=%u prepares=%u renders=%u/%u/%u ops=%u lookups=%u null=%u extents=%u filled_ways=%u chunk=%lu\n",inits,constructions,destructions,preloads,preloadMisses,pixelReads,asyncUsed,textureUpdates,textReads,prepares,renders,renderSkips,renderDraws,op,lookups,nulls,extents,hits,glyphTextureCacheGlyphDataCacheChunk);return 0;
}
