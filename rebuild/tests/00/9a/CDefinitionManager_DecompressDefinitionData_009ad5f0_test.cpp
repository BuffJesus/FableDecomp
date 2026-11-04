#include "fable_definition_manager.h"
#include "fable_data_input_stream.h"
#include "test_support/compressor_virtual_stubs.h"
#include <stdio.h>
#include <string.h>
const CDefinitionManager* g_DefCacheManager_0138E190;
unsigned long g_DefCacheOffset_0138E194, g_DefCacheSize_0138E198;
unsigned char g_DefCacheData_0138E19C[0x8000];
static int calls, clocks, inits, dtors, bad;
static const void* wantSource;
static unsigned long wantCompressed;
static const void* opened;
static unsigned long openedSize;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CACompressor::CACompressor(long n) { if(n!=0)bad=1; }
unsigned long FABLE_FASTCALL CCompressorZlib::GetRequiredWorkspaceSize() { return 0; }
void CCompressorZlib::Initialise() { ++inits; }
CCompressorZlib::~CCompressorZlib() { ++dtors; }
double FABLE_FASTCALL GFGetTime() { ++clocks;return clocks; }
unsigned long CCompressorZlib::InternalDecompress(const void* src,unsigned long n,void* out,unsigned long cap) {
 ++calls; if(src!=wantSource || n!=wantCompressed || out!=g_DefCacheData_0138E19C || cap!=0x8000)bad=2;
 unsigned short offsets[]={6,10,17}; memcpy(out,offsets,sizeof(offsets));return 29;
}
void CMemoryDataInputStream::OpenMemorySource(const void* p,unsigned long n){opened=p;openedSize=n;}
typedef _STL::pair<unsigned long,unsigned long> Entry;
typedef CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> > Map;
template<> const Entry* Map::UpperBound(const unsigned long& key) const {
 const Entry* p=begin();while(p!=end() && p->first<=key)++p;return p;
}
struct Access : CDefinitionManager {
 void Run(CMemoryDataInputStream& s,unsigned long n)const{DecompressDefinitionData(s,n);}
 void Setup(Entry* p,unsigned char* bytes){CompressedOffsets._M_start=p;CompressedOffsets._M_finish=p+3;CompressedDefData._M_start=bytes;}
};
static double store[2][sizeof(Access)/8+1];
int main(){
 unsigned char data[100]; Entry offsets[3];
 offsets[0].first=0;offsets[0].second=5;offsets[1].first=3;offsets[1].second=25;offsets[2].first=6;offsets[2].second=60;
 Access* a=reinterpret_cast<Access*>(store[0]);Access* b=reinterpret_cast<Access*>(store[1]);
 a->Setup(offsets,data);b->Setup(offsets,data);
 double rawStream[4];CMemoryDataInputStream& stream=*reinterpret_cast<CMemoryDataInputStream*>(rawStream);
 wantSource=data+5;wantCompressed=20;
 a->Run(stream,0);if(opened!=g_DefCacheData_0138E19C+6 || openedSize!=4 || calls!=1)return 1;
 a->Run(stream,1);if(opened!=g_DefCacheData_0138E19C+10 || openedSize!=7 || calls!=1)return 2;
 a->Run(stream,2);if(opened!=g_DefCacheData_0138E19C+17 || openedSize!=12 || calls!=1)return 3;
 wantSource=data+25;wantCompressed=35;
 a->Run(stream,3);if(openedSize!=4 || calls!=2)return 4;
 b->Run(stream,4);if(openedSize!=7 || calls!=3 || g_DefCacheManager_0138E190!=b)return 5;
 if(bad || clocks!=6 || inits!=3 || dtors!=3 || g_DefCacheOffset_0138E194!=25 || g_DefCacheSize_0138E198!=29)return 6;
 puts("DECOMPRESS_DEF_PASS");return 0;
}