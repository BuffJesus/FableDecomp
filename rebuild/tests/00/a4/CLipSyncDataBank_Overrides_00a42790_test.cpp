// Behaviour fixture for the CLipSyncDataBank overrides 0x00A42790..0x00A427F6 / 0x00A42940, linked against the
// provider object (it defines none of them). The bank is raw storage at the retail offsets the header pins; the
// CBankFile members the overrides forward to are test doubles.
#include "fable_lipsync_data_bank.h"
#include <stdio.h>
#include <string.h>

static int g_fail, g_updates, g_enumCalls;
static const void* g_enumThis;
static unsigned long g_enumIndex;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CBankFile::Update() { ++g_updates; }
CCharString CBankFile::GetEnumerationType(unsigned long index) const
{
    ++g_enumCalls; g_enumThis = this; g_enumIndex = index;
    CCharString result;
    return result;
}
CCharString::CCharString() { memset(this, 0, sizeof(*this)); }
CCharString::CCharString(const CCharString&) { memset(this, 0, sizeof(*this)); }
CCharString::~CCharString() {}

struct CLipSyncDataBankTest : CLipSyncDataBank
{
    bool DiskFile() { return CLipSyncDataBank::IsDiskFile(3, 4); }
    CCharString Enum(unsigned long i) const { return CLipSyncDataBank::GetEnumerationType(i); }
};

int main()
{
    static unsigned char storage[sizeof(CLipSyncDataBank)];
    memset(storage, 0, sizeof(storage));
    CLipSyncDataBankTest* bank = reinterpret_cast<CLipSyncDataBankTest*>(storage);

    CHECK(bank->CLipSyncDataBank::GetDataFormatVersion() == 0x70);
    storage[0x178] = 0;
    bank->CLipSyncDataBank::Update();
    storage[0x178] = 1;
    bank->CLipSyncDataBank::Update();
    CHECK(g_updates == 1);
    CHECK(!bank->DiskFile());
    {
        CCharString e = bank->Enum(9);
    }
    CHECK(g_enumCalls == 1 && g_enumThis == bank && g_enumIndex == 9);
    CHECK(sizeof(CCountedPointer<CBankStateBlock>) == 8);
    CPhonemeDataBank* phonemes = reinterpret_cast<CPhonemeDataBank*>(storage + 0x40);
    bank->CLipSyncDataBank::SetPhonemeBank(phonemes);
    CHECK(*reinterpret_cast<CPhonemeDataBank**>(storage + 0x17C) == phonemes);
    printf(g_fail ? "FAIL\n" : "CLIPSYNCDATABANK_OK\n");
    return g_fail != 0;
}
