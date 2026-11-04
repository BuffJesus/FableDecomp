#pragma once

#include "rebuild_abi.h"
#include "fable_string.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_tree.h"

class CDataInputStream;

// CDefString (Ego_r: 4 bytes, `long TablePos`): a definition name stored as
// its index in the global def string table.
class CDefString
{
public:
    // FableWin ??0CDefString@@QAE@ABVCCharString@@@Z: inline in retail, a
    // SetString call (CDefinitionManager::InstantiateDef 0x009ADC90 builds
    // three this way; the callee does not return `this`, so it is no ctor).
    CDefString(const CCharString& text)
    {
        SetString(text);
    }

    // FableWin ?SetString@CDefString@@QAEXABVCCharString@@@Z, retail 0x009D5060:
    // TablePos = GDefStringTable.AddString(text).
    void SetString(const CCharString& text);

    // FableWin ??0CDefString@@QAE@XZ; inline in retail (CParentDefClassBase's
    // InstantiationName in CFrontEndDef::Alloc 0x00433080): no table entry.
    CDefString()
        : TablePos(-1)
    {
    }

    // FableWin ?TransferBinaryIn@CDefString@@QAEXAAVCDataInputStream@@@Z, retail
    // 0x0044FC00 (a 4-byte read; the body is shared with other 4-byte transfers).
    void TransferBinaryIn(CDataInputStream& stream);

    operator CCharString();

    long TablePos;
};
FABLE_STATIC_ASSERT(sizeof(CDefString) == 4);

// CDefStringTable: Ego_r 0x40 bytes; retail 0x38 (the STLport map is 12
// bytes and the memory stream 0x18), offsets from AddString 0x009D4F90.
class CDefStringTable
{
public:
    // FableWin ??0CDefStringTable@@QAE@XZ, retail 0x009D4DB0 (GDefStringTable's
    // dynamic initializer 0x012240A0): empty, unlocked, RandomID = the CRC of
    // the current system time.
    CDefStringTable();
    // FableWin ??1CDefStringTable@@QAE@XZ, retail 0x009D4E90 (atexit 0x0122A780).
    ~CDefStringTable();

    // FableWin ?AddString@CDefStringTable@@QAEJABVCCharString@@@Z, retail 0x009D4F90.
    long AddString(const CCharString& text);
    // FableWin ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z, retail 0x009D49B0.
    CCharString GetString(long tablePos) const;
    // FableWin ?GetStringCRC@CDefStringTable@@QBEKJ@Z, retail 0x009D4A20.
    unsigned long GetStringCRC(long tablePos) const;
    // FableWin ?Unlock@CDefStringTable@@QAEXXZ, retail 0x009D4F10.
    void Unlock();
    // FableWin ?LoadTable@CDefStringTable@@QAEX_N@Z, retail 0x009D4C30 (GFMain
    // loads names.bin through it when compiled defs are in use), and
    // ?ReadTable@...@@QAEXAAVCDataInputStream@@@Z, retail 0x009D4AB0.
    // FableWin SetTableFilename; retail 009D4A40 tailcalls wide assignment.
    void SetTableFilename(const CWideString& filename);
    void LoadTable(bool readOnly);
    void SaveTable(bool lockAfterSave);
    void WriteTable(CDataOutputStream& output);
    void ReadTable(CDataInputStream& stream);

    _STL::map<CCharString, long> StringMap;     // +0x00
    CMemoryDataOutputStream StringStream;       // +0x0C (each entry: CRC, then the text)
    unsigned long StringCount;                  // +0x24
    unsigned long SharedCount;                  // +0x28
    unsigned long RandomID;                     // +0x2C (compiled def files record it; LoadBinaryDefinitions 0x009AFB90 checks it)
    CWideString TableFilename;                  // +0x30
    bool Locked;                                // +0x34
    bool ReadOnly;                              // +0x35
};
FABLE_STATIC_ASSERT(sizeof(CDefStringTable) == 0x38);

// Retail 0x013CA824 (name not recovered): AddString points it at the table
// stream's text, just past the stream's 4-byte header.
extern const char* g_DefStringTableText_013CA824;

// Ego_r `GDefStringTable` (dynamic initializer 0x0101AC00 in Ego_r); retail
// storage 0x013CA828.
extern CDefStringTable GDefStringTable;

FABLE_STATIC_ASSERT(offsetof(CDefStringTable, TableFilename) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CDefStringTable, Locked) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CDefStringTable, ReadOnly) == 0x35);
