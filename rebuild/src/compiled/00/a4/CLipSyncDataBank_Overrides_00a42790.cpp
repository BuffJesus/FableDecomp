// CLipSyncDataBank's small CBankFile overrides (retail 0x00A42790..0x00A427F6 and 0x00A42940).
#include "fable_lipsync_data_bank.h"

// 0x00A42790
unsigned long CLipSyncDataBank::GetDataFormatVersion()
{
    return 0x70;
}

// 0x00A427A0: the bank can be told to skip its per-frame update.
void CLipSyncDataBank::Update()
{
    if (!SkipUpdate)
        CBankFileAsync::Update();
}

// 0x00A427B0
void CLipSyncDataBank::SetPhonemeBank(CPhonemeDataBank* bank)
{
    PhonemeBank = bank;
}

// 0x00A427C0: lip-sync entries are never loose disk files.
bool CLipSyncDataBank::IsDiskFile(unsigned long, unsigned long)
{
    return false;
}

// 0x00A427E0
CCharString CLipSyncDataBank::GetEnumerationType(unsigned long index) const
{
    return CBankFileAsync::GetEnumerationType(index);
}

// 0x00A42940: nothing to prepare; only the by-value state block is released.
void CLipSyncDataBank::PrepareDataForInclusion(unsigned long, const CArray<CCharString>&,
    unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&)
{
}
