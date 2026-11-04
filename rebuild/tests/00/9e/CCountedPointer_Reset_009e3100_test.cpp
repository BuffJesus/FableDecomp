#include "test_support/bank_extended_virtual_boundaries.h"
#include "test_support/async_bank_lifetime_stubs.h"
#include <stdio.h>
#include <stdlib.h>
#include "fable_counted_pointer.h"
#include "fable_font_data_banks.h"
#include "test_support/bank_file_member_stubs.h"
#include "test_support/font_bank_state_stubs.h"
static int deleted;static void FABLE_FASTCALL DeleteOld(void*){++deleted;}
CBankFile::~CBankFile(){}
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long){return false;}
CBankFileAsync::~CBankFileAsync(){}
template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list() {}
void CFontDataBank::Close() {}
void CStreamingFontDataBank::Close() {}
CFontDataBank::~CFontDataBank(){}
CStreamingFontDataBank::~CStreamingFontDataBank(){}
int main(){
    CCountedPointer<CStreamingFontDataBank> pointer;
    CStreamingFontDataBank* first=reinterpret_cast<CStreamingFontDataBank*>(0x11110);
    CStreamingFontDataBank* second=reinterpret_cast<CStreamingFontDataBank*>(0x22220);
    pointer.Data=first;pointer.Info=new CCPPointerInfo(&DeleteOld,first);
    pointer.Reset(0);
    if(deleted!=1||pointer.Data||pointer.Info)return 1;
    pointer.Reset(second);
    CCPPointerInfo* info=pointer.Info;
    if(pointer.Data!=second||!info||info->RefCount!=1||info->Data!=second||!info->DeleteFunc)return 2;
    pointer.Data=first;CCPPointerInfo* sharedInfo=new CCPPointerInfo(&DeleteOld,first);pointer.Info=sharedInfo;
    pointer.Info->RefCount=2;
    pointer.Reset(second);
    if(deleted!=1||sharedInfo->RefCount!=1||pointer.Data!=second||!pointer.Info||pointer.Info->RefCount!=1)return 3;
    delete info;delete sharedInfo;delete pointer.Info;pointer.Data=0;pointer.Info=0;
    puts("FONT_RESET_009e3100_PASS");return 0;
}
