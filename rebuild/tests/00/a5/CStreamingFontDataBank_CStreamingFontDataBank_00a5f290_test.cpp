#include "test_support/bank_extended_virtual_boundaries.h"
#include "test_support/async_bank_lifetime_stubs.h"
#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_font_data_banks.h"
#include "test_support/bank_file_virtual_stubs.h"
#include "test_support/bank_file_cleanup_stubs.h"
static int fileBaseCalls,asyncBaseCalls,stringCalls,assignCalls;
CBankFile::CBankFile() { ++fileBaseCalls; stringCalls = 0; } CBankFile::~CBankFile() {}
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long) { return false; }
CBankFileAsync::CBankFileAsync() { ++asyncBaseCalls; } CBankFileAsync::~CBankFileAsync() {}
template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list() {}
CFontBankStateBlock::~CFontBankStateBlock() {}
CStreamingFontBankStateBlock::~CStreamingFontBankStateBlock() {}
CCharString::CCharString() : storage_(0) { ++stringCalls; }
CCharString::~CCharString() {}
const CCharString& CCharString::operator=(const char* value) { if (value && value[0]==0) ++assignCalls; return *this; }
CStreamingFontDataBank::~CStreamingFontDataBank() {}

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
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
void CStreamingFontDataBank::Close() {}

int main() {
    unsigned char storage[sizeof(CStreamingFontDataBank)];memset(storage,0xA5,sizeof(storage));
    CStreamingFontDataBank* bank=new (storage) CStreamingFontDataBank;
    if(bank!=reinterpret_cast<CStreamingFontDataBank*>(storage)||fileBaseCalls!=1||asyncBaseCalls!=1||stringCalls!=1||assignCalls!=1)return 1;
    if(!*reinterpret_cast<unsigned long*>(storage)||!*reinterpret_cast<unsigned long*>(storage+356))return 2;
    for(unsigned i=376;i<388;i+=4)if(*reinterpret_cast<unsigned long*>(storage+i))return 3;
    if(*reinterpret_cast<unsigned long*>(storage+388))return 4;
    if(*reinterpret_cast<unsigned long*>(storage+0x188)||*reinterpret_cast<unsigned long*>(storage+0x18c))return 5;
    puts("FONT_BANK_CTOR_00a5f290_PASS");return 0;
}
