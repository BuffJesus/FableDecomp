// CLipSyncDataBank's entry table: releasing the entries (0x00A42C00, inlined into Close and the destructor) and
// growing it as the bank file reports its size and entries. The STLport vector<CLipSyncDataBankEntry> members
// and helpers these instantiate are retail's out-of-line copies 0x00A43500..0x00A43F20.
#include <new>

#include "fable_stlport_vector.h"
#define FABLE_STLP_FILL_CALL FABLE_FASTCALL
#include "fable_stlport_vector_impl.h"
#include "fable_lipsync_data_bank.h"

// 0x00A42B80: the bank owns its resource bank. (Ego_r also builds a CSpeechAnalyser here; retail has none.)
CLipSyncDataBank::CLipSyncDataBank()
    : SkipUpdate(false), PhonemeBank(0)
{
    SetNonCached(false);
    ResourceBank.Reset(new CResourceBank);
}

// 0x00A42C00
void CLipSyncDataBank::CloseData()
{
    Entries.clear();
}

// 0x00A42CE0
CLipSyncDataBank::~CLipSyncDataBank()
{
    CloseData();
}

// 0x00A42DA0: the entries go, then the bank file itself closes (CBankFile's, not CBankFileAsync's).
void CLipSyncDataBank::Close()
{
    CloseData();
    CBankFile::Close();
}

// 0x00A42DF0: the entry's payload starts with the line's duration.
void CLipSyncDataBank::NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data)
{
    if (index >= Entries.size())
        Entries.resize(index + 1);
    float duration;
    memcpy(&duration, data.GetAsCArray(), sizeof(duration));
    Entries[index].Duration = duration;
    CBankFile::NotifyEntryAdded(id, index, data);
}

// 0x00A42E70: room for every entry the bank holds.
void CLipSyncDataBank::NotifyBankSize(unsigned long count, const CArray<CBankFileDataTypeCount>&)
{
    Entries.resize(count);
}
