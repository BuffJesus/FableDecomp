#include "test_support/bank_extended_virtual_boundaries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_font_data_banks.h"
#include "test_support/bank_file_member_stubs.h"

static char g_events[8];
static unsigned g_count;
static void Record(char event) { g_events[g_count++] = event; }

CBankFile::CBankFile() {}
CBankFile::~CBankFile() { Record('B'); }
bool CBankFile::OpenReadOnly(const CCharString&, unsigned long) { return false; }
void CFontBankStateBlock::GetDefault(CBankStateBlock&) {}
unsigned long CFontBankStateBlock::CreateCRC() { return 0; }
void CFontBankStateBlock::Set(const CBankStateBlock*) {}
CFontDataBank::CFontDataBank() {}
void CFontDataBank::Close() {}
void CFontDataBank::CloseFonts() { Record('C'); }

int main()
{
    CFontDataBank* bank = new CFontDataBank;
    bank->Fonts._M_start =
        static_cast<CFontDataBankEntry**>(malloc(sizeof(void*)));
    bank->Fonts._M_finish = bank->Fonts._M_start;
    bank->Fonts._M_end_of_storage._M_data = bank->Fonts._M_start + 1;
    bank->CFontDataBank::~CFontDataBank();
    if (g_count != 2 || g_events[0] != 'C' || g_events[1] != 'B')
        return 1;
    operator delete(bank);
    g_count = 0;
    bank = new CFontDataBank;
    delete bank;
    if (g_count != 2 || g_events[0] != 'C' || g_events[1] != 'B')
        return 2;
    CFontBankStateBlock* state = new CFontBankStateBlock;
    delete state;
    puts("FONT_DATA_BANK_DTOR_00a63b30_PASS");
    return 0;
}
