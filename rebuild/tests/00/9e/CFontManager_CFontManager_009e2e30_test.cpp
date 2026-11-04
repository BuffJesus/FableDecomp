#include "test_support/bank_extended_virtual_boundaries.h"
#include "../src/compiled/00/4a/CSymbolMap_ContainerLifetime_004ad4a0.cpp"
#include "test_support/async_bank_lifetime_stubs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_system_manager.h"
#include "fable_font_data_banks.h"
#include "test_support/bank_file_virtual_stubs.h"
#include "fable_symbol_map.h"
#include "fable_font_vertex_buffer.h"
#include "test_support/bank_file_cleanup_stubs.h"

static unsigned baseCount, staticBankCount, streamingBankCount, symbolCount;
static unsigned resizeCount, bufferCount, initialised;
static unsigned openCount, mapCount, streamingInitCount;
static unsigned resetStatic, resetStreaming, resetSymbols;
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++baseCount; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBankFile::CBankFile() {} CBankFile::~CBankFile() {}
CBankFileAsync::CBankFileAsync() {} CBankFileAsync::~CBankFileAsync() {}
template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list() {}
bool CBankFile::OpenReadOnly(const CCharString&, unsigned long flags) { ++openCount; return flags == 0x48; }
void CBankFile::CreateSymbolMap(CSymbolMap&, bool flag) const { if (!flag) ++mapCount; }
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CFontDataBank::CFontDataBank() { ++staticBankCount; }
CStreamingFontDataBank::CStreamingFontDataBank() { ++streamingBankCount; }
void CStreamingFontDataBank::InitStreamingEntry(CSymbolMap&) { ++streamingInitCount; }
CSymbolMap::CSymbolMap() { ++symbolCount; } 
CFontManager::~CFontManager() {}
CVertexBufferWin32::~CVertexBufferWin32() {}
bool CVertexBufferWin32::Initialise(EVertexType type,long count,long stride,unsigned long flags,EVertexBufferPool pool) {
 if (type==0x144 && count==300 && stride==0x1c && flags==8 && pool==1) ++initialised;
 return true;
}
template<> void CCountedPointer<CFontDataBank>::Reset(CFontDataBank* p) {Data=p;Info=0;++resetStatic;}
template<> void CCountedPointer<CStreamingFontDataBank>::Reset(CStreamingFontDataBank* p) {Data=p;Info=0;++resetStreaming;}
template<> void CCountedPointer<CSymbolMap>::Reset(CSymbolMap* p) {Data=p;Info=0;++resetSymbols;}
template<> void _STL::vector<CVertexBufferWin32*>::resize(size_t count,CVertexBufferWin32* const& value) {
 ++resizeCount; _M_start=(CVertexBufferWin32**)malloc(count*sizeof(CVertexBufferWin32*));
 for (unsigned i=0;i<count;++i) _M_start[i]=value;
 _M_finish=_M_start+count;_M_end_of_storage._M_data=_M_finish;
}
struct CFontManagerTestAccess {
 static unsigned count(const CFontManager& m) {return m.VertexBuffers.size();}
 static CVertexBufferWin32* at(const CFontManager& m,unsigned i) {return m.VertexBuffers[i];}
 static bool async(const CFontManager& m) {return m.LoadFontGlyphsAsynchronously;}
};

// Dependencies of the typed bank members; this fixture exercises construction.
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}
CPackedUIntArray::CPackedUIntArray() : PackedInts(0), Size(0), Bits(0), Bias(0) {}
void CFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CFontBankStateBlock::CreateCRC() { return 0; }
void CFontBankStateBlock::Set(const CBankStateBlock*) {}
void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CStreamingFontBankStateBlock::CreateCRC() { return 0; }
void CStreamingFontBankStateBlock::Set(const CBankStateBlock*) {}
CFontBankStateBlock::~CFontBankStateBlock() {}
CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}
CFontDataBank::~CFontDataBank() {}
CStreamingFontDataBank::~CStreamingFontDataBank() {}
void CFontDataBank::Close() {}
void CStreamingFontDataBank::Close() {}

int main() {
 unsigned long emptyStorage=0;const CCharString& empty=*reinterpret_cast<const CCharString*>(&emptyStorage);
 unsigned char storage[sizeof(CFontManager)];memset(storage,0xA5,sizeof(storage));
 CFontManager* manager=new (storage) CFontManager(empty,empty);
 if (!manager||staticBankCount!=1||streamingBankCount!=1||symbolCount!=2||resizeCount!=1||initialised!=8) return 1;
 if (resetStatic!=1||resetStreaming!=1||resetSymbols!=2||openCount||mapCount||streamingInitCount) return 2;
 if (CFontManagerTestAccess::count(*manager)!=8||!CFontManagerTestAccess::async(*manager)) return 3;
 for (unsigned i=0;i<8;++i) if(!CFontManagerTestAccess::at(*manager,i))return 4;
 unsigned char stringData[0x11];memset(stringData,0,sizeof(stringData));
 *reinterpret_cast<unsigned long*>(stringData+4)=5;
 unsigned long handleStorage=reinterpret_cast<unsigned long>(stringData);
 const CCharString& nonempty=*reinterpret_cast<const CCharString*>(&handleStorage);
 unsigned char secondStorage[sizeof(CFontManager)];memset(secondStorage,0xA5,sizeof(secondStorage));
 CFontManager* second=new (secondStorage) CFontManager(nonempty,nonempty);
 if (!second||staticBankCount!=2||streamingBankCount!=2||symbolCount!=4||resizeCount!=2||initialised!=16) return 5;
 if (resetStatic!=2||resetStreaming!=2||resetSymbols!=4||openCount!=2||mapCount!=2||streamingInitCount!=1) return 6;
 if (CFontManagerTestAccess::count(*second)!=8||!CFontManagerTestAccess::async(*second)) return 7;
 puts("FONT_CTOR_009e2e30_PASS");return 0;
}


// Construction/destruction fixture only. Dispatch is checked by the independent native gate.
unsigned char* CVertexBufferWin32::DoLock(unsigned long,unsigned long,unsigned long){abort();return 0;}
void CVertexBufferWin32::DoUnlock(){abort();}
bool CVertexBufferWin32::DoIsBusy() const{abort();return false;}
unsigned long CVertexBufferWin32::DoSizeof() const{abort();return 0;}
