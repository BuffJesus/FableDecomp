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
CFontDataBank::~CFontDataBank() {}

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
void CFontDataBank::Close() {}

int main() {
    unsigned char storage[sizeof(CFontDataBank)];memset(storage,0xA5,sizeof(storage));
    CFontDataBank* bank=new (storage) CFontDataBank;
    if(bank!=reinterpret_cast<CFontDataBank*>(storage)||fileBaseCalls!=1||asyncBaseCalls!=0||stringCalls!=1||assignCalls!=1)return 1;
    if(!*reinterpret_cast<unsigned long*>(storage)||!*reinterpret_cast<unsigned long*>(storage+272))return 2;
    for(unsigned i=288;i<300;i+=4)if(*reinterpret_cast<unsigned long*>(storage+i))return 3;
    if(*reinterpret_cast<unsigned long*>(storage+300))return 4;
    
    puts("FONT_BANK_CTOR_00a63630_PASS");return 0;
}
