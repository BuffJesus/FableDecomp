#include "test_support/bank_extended_virtual_boundaries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
class CDiskFileWin32 { public: ~CDiskFileWin32() {} };
#include "fable_game_text_bank.h"
#include "test_support/bank_file_virtual_stubs.h"
#include "test_support/bank_file_cleanup_stubs.h"
static int baseCalls,packedCalls,wideCalls,charCalls,cacheCalls;
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {++baseCalls;}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBankFile::~CBankFile() {}
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long) {return false;}
CPackedUIntArray::CPackedUIntArray():PackedInts(0),Size(0),Bits(0),Bias(0){++packedCalls;}
CWideString::CWideString():storage_(0){++wideCalls;} CWideString::~CWideString(){}
CCharString::CCharString():storage_(0){++charCalls;} CCharString::~CCharString(){}
CChecksumCacheFile::CChecksumCacheFile(long requested):BufferSize(requested),NeedsSaving(false){++cacheCalls;}
static unsigned long word(const unsigned char* p,unsigned offset){return *reinterpret_cast<const unsigned long*>(p+offset);}
class FixtureConcreteBank : public CBankFile { protected: virtual unsigned long GetDataFormatVersion() { return 0; } };
int main(){
 unsigned char storage[sizeof(CBankFile)];memset(storage,0xA5,sizeof(storage));
 CBankFile* bank=new(storage) FixtureConcreteBank;
 if(bank!=reinterpret_cast<CBankFile*>(storage)||baseCalls!=1||packedCalls!=4||wideCalls!=2||charCalls!=2||cacheCalls!=1)return 1;
 if(!word(storage,0)||word(storage,4)||word(storage,0x7c)||word(storage,0x80)||word(storage,0x84)||word(storage,0x88))return 2;
 for(unsigned offset=8;offset<0x38;offset+=4)if(word(storage,offset))return 3;
 for(unsigned offset=0x38;offset<0x78;offset+=0x10){if(word(storage,offset)||word(storage,offset+4)||storage[offset+8]||word(storage,offset+12))return 4;}
 if(word(storage,0x98)!=1||word(storage,0x9c)!=0x73||word(storage,0xa0)||word(storage,0xa4)||word(storage,0xa8))return 5;
 for(unsigned offset=0xac;offset<=0xc4;offset+=0xc){
   unsigned long node=word(storage,offset);if(!node||word(storage,offset+4))return 6;
   if(*(unsigned char*)node||word((const unsigned char*)node,4)||word((const unsigned char*)node,8)!=node||word((const unsigned char*)node,12)!=node)return 7;
   free((void*)node);
 }
 for(unsigned offset=0xd0;offset<=0xd8;offset+=4)if(word(storage,offset))return 8;
 if(storage[0xdd]||word(storage,0xe0)||word(storage,0xe4)||word(storage,0xe8)||word(storage,0xec)||word(storage,0xf0))return 9;
 if(word(storage,0x108)!=0x10000||storage[0x10c])return 10;
 free((void*)word(storage,0xfc));
 puts("BANKFILE_CTOR_009cd480_PASS");return 0;
}
