#include "test_support/bank_extended_virtual_boundaries.h"
#include "test_support/async_bank_lifetime_stubs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_font_data_banks.h"
#include "test_support/bank_file_member_stubs.h"

static char g_events[32];
static unsigned g_eventCount;
static void Record(char value)
{
    g_events[g_eventCount++] = value;
    g_events[g_eventCount] = 0;
}

CBankFile::CBankFile() {}
CBankFile::~CBankFile() { Record('B'); }
bool CBankFile::OpenReadOnly(const CCharString&, unsigned long) { return false; }
template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list() {}
CBankFileAsync::CBankFileAsync() {}
CBankFileAsync::~CBankFileAsync() { Record('A'); }
void CStreamingFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CStreamingFontBankStateBlock::CreateCRC() { return 0; }
void CStreamingFontBankStateBlock::Set(const CBankStateBlock*) {}
CStreamingFontDataBank::CStreamingFontDataBank() {}
void CStreamingFontDataBank::Close() {}
void CStreamingFontDataBank::CloseFonts() { Record('C'); }

static void FABLE_FASTCALL DeleteEntry(void*) { Record('D'); }

int main()
{
    void* storage = malloc(sizeof(CStreamingFontDataBank));
    CStreamingFontDataBank* bank = new (storage) CStreamingFontDataBank;
    int fakeEntry = 1;
    bank->streamingEntry.Info = new CCPPointerInfo(&DeleteEntry, &fakeEntry);
    bank->streamingEntry.Data = reinterpret_cast<CBankFileAsyncEntry*>(&fakeEntry);
    bank->Fonts._M_start = static_cast<CStreamingFontDataBankEntry**>(malloc(sizeof(void*)));
    bank->Fonts._M_finish = bank->Fonts._M_start;
    bank->Fonts._M_end_of_storage._M_data = bank->Fonts._M_start + 1;
    g_eventCount = 0;
    g_events[0] = 0;
    bank->~CStreamingFontDataBank();
    if (strcmp(g_events, "CDAB") != 0)
        return 1;
    bank = new (storage) CStreamingFontDataBank;
    CCPPointerInfo* shared = new CCPPointerInfo(&DeleteEntry, &fakeEntry);
    shared->RefCount = 2;
    bank->streamingEntry.Info = shared;
    bank->streamingEntry.Data = reinterpret_cast<CBankFileAsyncEntry*>(&fakeEntry);
    g_eventCount = 0;
    g_events[0] = 0;
    bank->~CStreamingFontDataBank();
    if (strcmp(g_events, "CAB") != 0 || shared->RefCount != 1)
        return 2;
    delete shared;
    free(storage);
    bank = new CStreamingFontDataBank;
    g_eventCount = 0;
    g_events[0] = 0;
    delete bank;
    if (strcmp(g_events, "CAB") != 0)
        return 3;
    CStreamingFontBankStateBlock* state = new CStreamingFontBankStateBlock;
    delete state;
    puts("STREAMING_FONT_BANK_DTOR_00a5f3a0_PASS");
    return 0;
}
