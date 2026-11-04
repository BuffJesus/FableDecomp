#pragma once

// Hand-owned retail font interfaces. Keep these out of donor-header regeneration.



// Curated retail layout: constructor 00A63630, PeekFontInfo 00A635E0, GetFontInfo 00A635D0.
// Ego_r's larger bank base/CArray layout does not describe the retail binary.
#include "fable_string.h"
#include "fable_counted_pointer.h"
#include "fable_game_text_bank.h"
#include "fable_stlport_vector.h"
#include "fable_font_state_blocks.h"
class CFontDataBankEntry;
class CFontBank;
class CBankFileAsyncEntry;
class CSymbolMap;
class CFontDataBank : public CBankFile
{
public:
    CFontDataBank();
    virtual ~CFontDataBank();
    virtual void Close();
    const CFontDataBankEntry* PeekFontInfo(unsigned long index) const;
    CFontDataBankEntry* GetFontInfo(unsigned long index) const;
    // Ego_r ?LoadFont@CFontDataBank@@QAEPAVCFontBank@@K@Z, retail 0x00A63350.
    CFontBank* LoadFont(unsigned long index);
    CFontBankStateBlock StateBlock;
    _STL::vector<CFontDataBankEntry*> Fonts;
    CCharString Filename;
protected:
    virtual unsigned long GetDataFormatVersion();
    virtual void NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&);
    virtual void NotifyEntryRemoved(unsigned long);
    virtual void PrepareDataForInclusion(unsigned long, const CArray<CCharString>&, unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&);
    virtual bool IsDiskFile(unsigned long, unsigned long);
    virtual CDateAndTime GetLastModifiedTime(const CCharString&);
    virtual CCharString GetEnumerationType(unsigned long) const;
private:
    void CloseFonts();
    friend struct CFontDataBankCloseFontsTestAccess;
};
FABLE_STATIC_ASSERT(sizeof(CFontDataBank) == 0x130);
FABLE_STATIC_ASSERT(offsetof(CFontDataBank, StateBlock) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CFontDataBank, Fonts) == 0x120);
FABLE_STATIC_ASSERT(offsetof(CFontDataBank, Filename) == 0x12C);



// Curated retail layout: constructor 00A5F290, PeekFontInfo 00A5F090, GetFontInfo 00A5F0A0.
// Ego_r's larger bank base/CArray layout does not describe the retail binary.
#include "fable_string.h"
#include "fable_counted_pointer.h"
#include "fable_stlport_vector.h"
class CBankFileAsyncData;
class CStreamingFontDataBankEntry;
class CBankFileAsyncEntry;
class CStreamingFontDataBank : public CBankFileAsync
{
public:
    CStreamingFontDataBank();
    virtual ~CStreamingFontDataBank();
    virtual void Close();
    void InitStreamingEntry(CSymbolMap& symbols);
    // Ego_r ?ReadData@CStreamingFontDataBank@@QAEXPAXKK@Z, retail 0x00A5EF30: synchronous read from the
    // streaming entry.
    void ReadData(void* data, unsigned long offset, unsigned long size);
    // Ego_r ?StreamGlyph@CStreamingFontDataBank@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@PAXKK_N@Z, retail
    // 0x00A5F010: asynchronous read when the font manager loads glyphs asynchronously, else synchronous.
    CCountedPointer<CBankFileAsyncData> StreamGlyph(void* data, unsigned long offset, unsigned long size, bool synchronous);
    const CStreamingFontDataBankEntry* PeekFontInfo(unsigned long index) const;
    CStreamingFontDataBankEntry* GetFontInfo(unsigned long index) const;
    CStreamingFontBankStateBlock StateBlock;
    _STL::vector<CStreamingFontDataBankEntry*> Fonts;
    CCharString Filename;
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;
protected:
    virtual unsigned long GetDataFormatVersion();
    virtual void NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&);
    virtual void NotifyEntryRemoved(unsigned long);
    virtual void PrepareDataForInclusion(unsigned long, const CArray<CCharString>&, unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&);
    virtual bool IsDiskFile(unsigned long, unsigned long);
    virtual CDateAndTime GetLastModifiedTime(const CCharString&);
    virtual CCharString GetEnumerationType(unsigned long) const;
    virtual unsigned long AdjustUpdateEntries();
private:
    void CloseFonts();
    friend struct CStreamingFontDataBankCloseFontsTestAccess;
};
FABLE_STATIC_ASSERT(sizeof(CStreamingFontDataBank) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontDataBank, StateBlock) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontDataBank, Fonts) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontDataBank, Filename) == 0x184);

#ifndef FABLE_ENGINE_CFONTDATABANKENTRY_H
#define FABLE_ENGINE_CFONTDATABANKENTRY_H
// Curated from the generated Ego_r layout; AcquireFont confirmed by retail/PDB.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CFontDataBankEntry = 0x18.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_counted_pointer.h"

class CCPPointerInfo;
#include "engine/CFontBank.h"
class CFontDataBank;

#include "fable_base_class.h"
#pragma pack(push, 1)
// Ego_r: CBaseClassNonCopyable base at +0 (the ~CFontDataBankEntry 0x00A63480
// tail-jumps to the folded base destructor).
class CFontDataBankEntry : public CBaseClassNonCopyable {
public:
    // Ego_r ??0CFontDataBankEntry@@QAE@KKPAVCFontDataBank@@@Z; inline in retail
    // CFontDataBank::NotifyEntryAdded 0x00A636C0.
    CFontDataBankEntry(unsigned long type, unsigned long index, CFontDataBank* bank)
        : Bank(bank), Index(index), DataType(type) {}
    CCountedPointer<CFontBank> AcquireFont() const;
    virtual ~CFontDataBankEntry();
    virtual bool IsLoaded() const;
    virtual bool CanUnload() const;
    virtual void Unload();
    mutable CCountedPointer<CFontBank> Font; // +0x04
    CFontDataBank*  Bank;                                    // +0x0c
    unsigned long   Index;                                   // +0x10
    unsigned long   DataType;                                // +0x14
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CFontDataBankEntry) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CFontDataBankEntry, Font) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CFontDataBankEntry, Bank) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CFontDataBankEntry, Index) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CFontDataBankEntry, DataType) == 0x14);

#endif // FABLE_ENGINE_CFONTDATABANKENTRY_H

#ifndef FABLE_ENGINE_CSTREAMINGFONTDATABANKENTRY_H
#define FABLE_ENGINE_CSTREAMINGFONTDATABANKENTRY_H
// Curated from the generated Ego_r layout; AcquireFont confirmed by retail/PDB.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CStreamingFontDataBankEntry = 0x18.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_counted_pointer.h"

class CCPPointerInfo;
#include "engine/CFontBank.h"
class CStreamingFontDataBank;

#include "fable_base_class.h"
#pragma pack(push, 1)
// Ego_r: CBaseClassNonCopyable base at +0, as for CFontDataBankEntry (retail
// CStreamingFontDataBank::NotifyEntryAdded 0x00A5F430 calls its constructor
// 0x0099A2F0; ~CStreamingFontDataBankEntry 0x00A5F150 tail-jumps to 0x0099A300).
class CStreamingFontDataBankEntry : public CBaseClassNonCopyable {
public:
    // Inline in retail CStreamingFontDataBank::NotifyEntryAdded 0x00A5F430.
    CStreamingFontDataBankEntry(unsigned long type, unsigned long index, CStreamingFontDataBank* bank)
        : Bank(bank), Index(index), DataType(type) {}
    CCountedPointer<CFontBank> AcquireFont() const;
    virtual ~CStreamingFontDataBankEntry();
    virtual bool IsLoaded() const;
    virtual bool CanUnload() const;
    virtual void Unload();
    mutable CCountedPointer<CFontBank> Font; // +0x04
    CStreamingFontDataBank* Bank;                            // +0x0c
    unsigned long           Index;                           // +0x10
    unsigned long           DataType;                        // +0x14
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CStreamingFontDataBankEntry) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontDataBankEntry, Font) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontDataBankEntry, Bank) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontDataBankEntry, Index) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CStreamingFontDataBankEntry, DataType) == 0x14);

#endif // FABLE_ENGINE_CSTREAMINGFONTDATABANKENTRY_H
