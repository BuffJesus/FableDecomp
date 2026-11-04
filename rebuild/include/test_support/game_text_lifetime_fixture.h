#include "fable_game_text_bank.h"
#include "test_support/async_bank_lifetime_stubs.h"
#include "test_support/bank_file_member_stubs.h"
#include <stdio.h>
#include <stdlib.h>
#include <new>

static unsigned int releases, asyncDestructs;
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const { ++releases; }
bool CResourceList::HandleFailedAllocation(unsigned long) { return false; }
void CResourceList::RemoveResource(CResource&) { abort(); }
void CResourceList::SetMaximumMemory(long value) { MaximumMemory=value; }
CBankFile::CBankFile() {}
CBankFile::~CBankFile() {}
CBankFileAsync::CBankFileAsync() {}
CBankFileAsync::~CBankFileAsync() { ++asyncDestructs; }
template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list() {}
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long) { return false; }
struct Probe : CIVCountedPointeeBase {
    virtual void DeleteCIVCountedPointeeBase() const { ++releases; }
};
#define CHECK(x) do { if(!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    Probe text,group;
    NGameText::CDataBank* bank=new NGameText::CDataBank;
    CHECK(bank->ResourceList.MaximumMemory==0x10000 && bank->ResourceList.UnloadDelay==0);
    CHECK(bank->LoadingQueue.MaximumMemory==0x7fffffff && bank->LoadingQueue.UnloadDelay==1);
    CHECK(!bank->Entries._M_start && !bank->TextEntryBuffer._M_start && !bank->GroupEntryBuffer._M_start && !bank->Narrators._M_start && !bank->Seed);
    NGameText::CDataBankTextEntry* te=static_cast<NGameText::CDataBankTextEntry*>(malloc(sizeof(*te)));
    new(te) NGameText::CDataBankTextEntry;
    te->PText.data_=reinterpret_cast<NGameText::CTextEntry*>(&text);
    bank->TextEntryBuffer._M_start=te;
    bank->TextEntryBuffer._M_finish=te+1;
    bank->TextEntryBuffer._M_end_of_storage._M_data=te+1;
    NGameText::CDataBankGroupEntry* ge=static_cast<NGameText::CDataBankGroupEntry*>(malloc(sizeof(*ge)));
    new(ge) NGameText::CDataBankGroupEntry;
    ge->PGroup.data_=reinterpret_cast<NGameText::CGroupEntry*>(&group);
    bank->GroupEntryBuffer._M_start=ge;
    bank->GroupEntryBuffer._M_finish=ge+1;
    bank->GroupEntryBuffer._M_end_of_storage._M_data=ge+1;
    CBankFile* primary=bank;
    primary->Close();
    CHECK(releases==2 && bank->TextEntryBuffer._M_finish==te && bank->GroupEntryBuffer._M_finish==ge);
    primary->Close();
    CHECK(releases==2);
    delete primary;
    CHECK(releases==2 && asyncDestructs==1);
    CCountedPointer<NGameText::CDataBank> owner;
    owner.Reset(new NGameText::CDataBank);
    text.IVRefCount=1;
    te=static_cast<NGameText::CDataBankTextEntry*>(malloc(sizeof(*te)));
    new(te) NGameText::CDataBankTextEntry;
    te->PText.data_=reinterpret_cast<NGameText::CTextEntry*>(&text);
    owner->TextEntryBuffer._M_start=te;
    owner->TextEntryBuffer._M_finish=te+1;
    owner->TextEntryBuffer._M_end_of_storage._M_data=te+1;
    owner.Reset(0);
    CHECK(owner.IsNull() && !owner.Info && releases==3 && asyncDestructs==2);
    puts("GAME_TEXT_LIFETIME_PASS nativeCtor=2 nativeResourceBank=2 virtualClose=2 virtualDelete=1 nativeCountedReset=1 populatedDestructor=1");
    return 0;
}

#include "../../src/compiled/00/9c/NGameText_CDataBank_Constructor_009c85a0.cpp"

#ifndef FABLE_TEXT_BANK_TEST_RESOURCE
#include "../../src/compiled/00/9f/CResourceBank_Lifetime_009fc5f0.cpp"
#endif

#ifndef FABLE_TEXT_BANK_TEST_BUFFERS
#include "../../src/compiled/00/9c/NGameText_CDataBank_CloseTextAndGroups_009c8660.cpp"
#endif

#ifndef FABLE_TEXT_BANK_TEST_DTOR
#include "../../src/compiled/00/9c/NGameText_CDataBank_Destructor_009c87c0.cpp"
#endif

#ifndef FABLE_TEXT_BANK_TEST_RESET
#include "../../src/compiled/00/40/CCountedPointer_PhysicsFacesSegment_Reset_00403e40.cpp"
#endif
