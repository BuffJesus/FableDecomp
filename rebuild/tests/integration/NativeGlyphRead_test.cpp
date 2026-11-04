#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_font_instances.h"
#include "fable_data_input_stream.h"
#include "fable_string.h"
#include "test_support/data_input_stream_stubs.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned row;
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"GLYPH_READ_FAILURE row=%u line=%d\n",row,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
struct Event{unsigned kind,size,position,chunk;};
class GlyphStream:public CDataInputStream{
public:
 const unsigned char* payload;unsigned start,supplied,chunkSize,nextCalls,unbufferedCalls,events,lastChunk;Event trace[128];
 GlyphStream(const unsigned char* bytes,unsigned position,unsigned initial,unsigned chunk):payload(bytes),start(position),supplied(initial),chunkSize(chunk),nextCalls(0),unbufferedCalls(0),events(0),lastChunk(position){
  StreamPos=position;StreamSize=position+22;CurrentSourcePtr=const_cast<unsigned char*>(bytes);SourceChunkStreamPos=position;SourceChunkBytesRemaining=initial;memset(trace,0,sizeof(trace));
 }
 void Record(unsigned kind,unsigned size){CHECK(events<128);trace[events].kind=kind;trace[events].size=size;trace[events].position=StreamPos-start;trace[events++].chunk=SourceChunkBytesRemaining;}
 virtual void InternalReadNextSourceChunk(const fable_u8*& source,long& size){
  CHECK(chunkSize && supplied<22 && StreamPos==start+supplied && SourceChunkStreamPos==StreamPos && (void*)&source==(void*)&CurrentSourcePtr && &size==&SourceChunkBytesRemaining);
  Record(2,0);lastChunk=start+supplied;unsigned available=22-supplied;if(available>chunkSize)available=chunkSize;source=payload+supplied;size=available;supplied+=available;++nextCalls;
 }
 virtual bool NeedsBufferedRead(long size){CHECK(size>0 && size<=4 && StreamPos==start+supplied);Record(1,size);return chunkSize!=0;}
 virtual void InternalReadUnbuffered(fable_u8* data,long size){CHECK(!chunkSize && size>0 && supplied+(unsigned)size<=22 && StreamPos==start+supplied);Record(3,size);memcpy(data,payload+supplied,size);supplied+=size;++unbufferedCalls;}
};
typedef void(CGlyph::*ReadMethod)(CDataInputStream&);
extern "C" void __fastcall OverflowRead(CDataInputStream*,void*,void*,long);
#pragma comment(linker,"/alternatename:@OverflowRead@16=?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z")
struct Result{unsigned char glyph[24];unsigned position,remaining,chunkPosition,pointerEnd,next,unbuffered,count;Event events[128];};
static Result Run(ReadMethod method,const unsigned char* payload,unsigned start,unsigned initial,unsigned chunk){
 struct Guarded{unsigned char before[16];CGlyph glyph;unsigned char after[16];} target;memset(&target,0xa5,sizeof(target));Result result;memset(&result,0,sizeof(result));
 {GlyphStream stream(payload,start,initial,chunk);unsigned status=_statusfp(),control=_controlfp(0,0);(target.glyph.*method)(stream);CHECK(_statusfp()==status && _controlfp(0,0)==control);
  CHECK(!memcmp(&target.glyph,payload,22));const unsigned char* bytes=(const unsigned char*)&target.glyph;CHECK(bytes[22]==0xa5 && bytes[23]==0xa5);for(unsigned i=0;i<16;++i)CHECK(target.before[i]==0xa5 && target.after[i]==0xa5);
  CHECK(stream.StreamPos==start+22 && stream.StreamSize==(long)(start+22) && stream.SourceChunkBytesRemaining==0 && stream.supplied==22);
  if(stream.unbufferedCalls)CHECK(stream.CurrentSourcePtr==0 && stream.SourceChunkStreamPos==0);else CHECK(stream.CurrentSourcePtr==payload+22 && stream.SourceChunkStreamPos==stream.lastChunk);
  CHECK((chunk==0 && initial<22)?stream.unbufferedCalls>0:stream.unbufferedCalls==0);
  memcpy(result.glyph,&target.glyph,24);result.position=stream.StreamPos;result.remaining=stream.SourceChunkBytesRemaining;result.chunkPosition=stream.SourceChunkStreamPos;result.pointerEnd=stream.CurrentSourcePtr?22:0;result.next=stream.nextCalls;result.unbuffered=stream.unbufferedCalls;result.count=stream.events;memcpy(result.events,stream.trace,sizeof(result.events));
 }
 return result;
}
int main(int argc,char** argv){
 CHECK(argc==1 || argc==2);ReadMethod original=0;unsigned char* code=0;
 if(argc==2){unsigned char bytes[654];FILE* file=fopen(argv[1],"rb");CHECK(file && fread(bytes,1,654,file)==654 && fgetc(file)==EOF);CHECK(!fclose(file));code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code);memcpy(code,bytes,654);unsigned calls[]={0x53,0xa7,0xfc,0x151,0x1a8,0x201,0x25d};for(unsigned i=0;i<7;++i)*(unsigned long*)(code+calls[i])=(unsigned long)(void*)OverflowRead-((unsigned long)code+calls[i]+4);CHECK(sizeof(original)==4);memcpy(&original,&code,4);}
 const unsigned patterns[]={0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xff812345,1,0x7fffff,0x800000,0x7f7fffff,0x3faaaaab,0x4f000000,0x12345678};const unsigned short shorts[]={0x8000,0xffff,0,0x7fff};const unsigned chunks[]={0,1,2,3,4,7,22};const unsigned positions[]={0,7,256,0x7fffffe9};unsigned originalCases=0;
 for(unsigned seed=0;seed<16;++seed)for(unsigned alignment=0;alignment<4;++alignment)for(unsigned p=0;p<4;++p)for(unsigned mode=0;mode<7;++mode)for(unsigned initial=0;initial<=22;++initial){
  unsigned char storage[4+22+16],before[sizeof(storage)];memset(storage,0xcd,sizeof(storage));unsigned char* data=storage+alignment;unsigned order[]={seed,(seed+3)%16,(seed+7)%16,(seed+11)%16};for(unsigned i=0;i<4;++i)memcpy(data+4*i,&patterns[order[i]],4);for(unsigned i=0;i<3;++i)memcpy(data+16+2*i,&shorts[(seed+i)%4],2);memcpy(before,storage,sizeof(storage));
  Result candidate=Run(&CGlyph::Read,data,positions[p],initial,chunks[mode]);if(original){Result native=Run(original,data,positions[p],initial,chunks[mode]);CHECK(!memcmp(&candidate,&native,sizeof(Result)));++originalCases;}CHECK(!memcmp(before,storage,sizeof(storage)));++row;
 }
 CHECK(row==41216);if(code)CHECK(VirtualFree(code,0,0x8000));printf("NATIVE_GLYPH_READ_PASS cases=%u original=%u float_patterns=16 alignments=4 positions=4 windows=23 chunk_modes=7 padding=preserved\n",row,originalCases);return 0;
}

// Unused constructor-TU/file/output-stream dependencies fail closed in this read-only fixture.
char g_FableEmptyNarrowString_0129AAF4[1]={0};
void CDataInputStream::SetPosition(unsigned long){abort();}
CFileDataInputStream::~CFileDataInputStream(){abort();}
CMemoryDataInputStream::~CMemoryDataInputStream(){abort();}
template<> void _STL::vector<char>::resize(unsigned,const char&){abort();}
