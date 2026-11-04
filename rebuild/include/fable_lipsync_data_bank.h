#pragma once
// Hand-owned (retail model): CLipSyncDataBank, the bank of per-line lip-sync entries, and its 8-byte entry.
// Ego_r.pdb: CLipSyncDataBank 0x200 = CBankFileAsync + Entries, ResourceBank, SpeechAnalyser, SkipUpdate, PhonemeBank.
// Retail has no SpeechAnalyser (editor-only): the constructor 0x00A42B80 initialises Entries +0x164 (STLport
// 12-byte array), ResourceBank +0x170, SkipUpdate +0x178 and PhonemeBank +0x17C; Ego_r's offsets are 0x74 (base)
// + 0x4 (CArray) larger. Retail vtable 0x0129D3D4 (RTTI .?AVCLipSyncDataBank@@), CBankFile slot order:
// ??_G 0x00A43FD0, Close 0x00A42DA0, Update 0x00A427A0, GetDataFormatVersion 0x00A42790, NotifyBankSize 0x00A42E70,
// NotifyEntryAdded 0x00A42DF0, PrepareDataForInclusion 0x00A42940, IsDiskFile 0x00A427C0,
// GetEnumerationType 0x00A427E0. Partial view: only members with a retail body are declared. The entry table
// (CloseData 0x00A42C00, destructor 0x00A42CE0) and its vector<CLipSyncDataBankEntry> family 0x00A43500..0x00A43FF0
// are CLipSyncDataBank_Entries_00a42c00.cpp.
#include "fable_game_text_bank.h"
#include "fable_iv_counted_pointer.h"

class CPhonemeDataBank;

// Ego_r CLipSyncDataBankEntry 0x8: the loaded sample data and the line's duration (seconds).
class CLipSyncDataBankEntry
{
public:
    class CData;

    // Retail builds the default entry inline (CLipSyncDataBank::NotifyEntryAdded 0x00A42DF0): no data, zero
    // duration. Copying and destruction are the members'.
    CLipSyncDataBankEntry()
        : Duration(0.0f)
    {
    }

private:
    friend class CLipSyncDataBank;
    friend struct CLipSyncDataBankLayoutCheck;
    CIVCountedPointer<CData> Data;   // +0x00
    float Duration;                  // +0x04
};

// Ego_r CLipSyncDataBankEntry::CData 0x34: a CResource (LipSyncData, FileData, Owner, Bank). Partial view: the
// entries only share and release it.
class CLipSyncDataBankEntry::CData : public CResource
{
};

class CLipSyncDataBank : public CBankFileAsync
{
public:
    CLipSyncDataBank();
    virtual ~CLipSyncDataBank();
    virtual void Close();
    virtual void Update();
    virtual unsigned long GetDataFormatVersion();
    void SetPhonemeBank(CPhonemeDataBank* bank);

protected:
    virtual void NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&);
    virtual void NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&);
    virtual void PrepareDataForInclusion(unsigned long, const CArray<CCharString>&,
        unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&);
    virtual bool IsDiskFile(unsigned long, unsigned long);
    virtual CCharString GetEnumerationType(unsigned long) const;

private:
    void CloseData();

    friend struct CLipSyncDataBankLayoutCheck;
    CArray<CLipSyncDataBankEntry> Entries;           // +0x164
    CCountedPointer<CResourceBank> ResourceBank;     // +0x170
    bool SkipUpdate;                                 // +0x178
    CPhonemeDataBank* PhonemeBank;                   // +0x17C
};

struct CLipSyncDataBankLayoutCheck
{
    FABLE_STATIC_ASSERT(sizeof(CLipSyncDataBankEntry) == 0x8);
    FABLE_STATIC_ASSERT(offsetof(CLipSyncDataBankEntry, Duration) == 0x4);
    FABLE_STATIC_ASSERT(offsetof(CLipSyncDataBank, Entries) == 0x164);
    FABLE_STATIC_ASSERT(offsetof(CLipSyncDataBank, ResourceBank) == 0x170);
    FABLE_STATIC_ASSERT(offsetof(CLipSyncDataBank, SkipUpdate) == 0x178);
    FABLE_STATIC_ASSERT(offsetof(CLipSyncDataBank, PhonemeBank) == 0x17C);
};
FABLE_STATIC_ASSERT(sizeof(CLipSyncDataBank) == 0x180);
