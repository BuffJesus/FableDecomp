#pragma once
// Hand-owned (retail model): CWADBank and CWADFile, a CAFile view of one entry in a WAD bank.
// Ego_r.pdb: CWADFile 0x10 = CAFile base (vptr), private CBankFileAsync& Bank +0x4, private unsigned long Index +0x8,
// private long FilePos +0xC. Retail vtable 0x0129CF8C (RTTI .?AVCWADFile@@), 16 slots in CAFile order:
// ??_G 0x00A39DF0, Open 0x00A39DC0, Close 0x00A39BA0, Read 0x00A39BB0, Write 0x00A39BE0, SetPosition 0x00A39BF0,
// MovePosition 0x00A39C00, GetPosition 0x00A39C10, SetLength 0x00A39DD0, GetLength 0x00A39C20, IsOpen 0x00A39DE0,
// GetPathname 0x00A39C40, IsIOFinished 0x00A39C30, GetAlignment 0x00A39C70, Clone 0x00A39C80,
// GetFileInformation 0x00A39D20. Constructor 0x00A39D80, private InternalOpen 0x00A39CC0.
// CWADBankStateBlock (Ego_r 0x8): retail vtable 0x0129CFD0, ??_G 0x00A3A360, GetDefault 0x00A3A2C0,
// CreateCRC 0x00A3A2D0, Set 0x00A3A2E0.
// The engine/CWADFile.h, engine/CWADBank.h and engine/CWADBankStateBlock.h generated views are shims of this header.
#include "fable_filesystem.h"
#include "fable_game_text_bank.h"
#include "fable_font_state_blocks.h"

// Ego_r CWADBank 0x1D8 : public CBankFileAsync. Partial view: only the base is modelled (no size assert);
// AddFile overloads, constructors and the GetDataFormatVersion override are not declared until a body lands.
class CWADBank : public CBankFileAsync
{
};

class CWADFile : public CAFile
{
public:
    CWADFile(CWADBank& bank, const CWideString& pathname, FileAccessType accessType, unsigned long flags);
    // Inline in retail: Clone 0x00A39C80 expands it (CAFile base, vptr, three member copies).
    CWADFile(const CWADFile& other)
        : Bank(other.Bank), Index(other.Index), FilePos(other.FilePos)
    {
    }

    virtual void Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags);
    virtual void Close();
    virtual void Read(void* data, long size, CASuspendableProcess* process);
    virtual void Write(const void* data, long size, CASuspendableProcess* process);
    virtual void SetPosition(long position);
    virtual void MovePosition(long offset);
    virtual long GetPosition() const;
    virtual void SetLength(long length);
    virtual long GetLength() const;
    virtual bool IsOpen() const;
    virtual CWideString GetPathname() const;
    virtual bool IsIOFinished() const;
    virtual long GetAlignment() const;
    virtual CAFile* Clone() const;
    virtual CFileInformation GetFileInformation();

private:
    friend struct CWADFileLayoutCheck;
    void InternalOpen(const CWideString& pathname, FileAccessType accessType, unsigned long flags);
    CWADFile& operator=(const CWADFile&);   // Ego_r declares it; a reference member makes it unassignable

    CBankFileAsync& Bank;   // +0x04
    unsigned long Index;    // +0x08
    long FilePos;           // +0x0C
};

struct CWADFileLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CWADFile, Index) == 0x8);
    FABLE_STATIC_ASSERT(offsetof(CWADFile, FilePos) == 0xC);
};
FABLE_STATIC_ASSERT(sizeof(CWADFile) == 0x10);

// A WAD bank has no load state: every override is empty and the CRC is 0. The destructor is implicit (its
// ??_G 0x00A3A360 only restores the CBankStateBlock vptr); retail emits it with the vtable in a constructing unit.
class CWADBankStateBlock : public CBankStateBlock
{
public:
    struct CStateBlock {};   // Ego_r: empty (1 byte)

    virtual void GetDefault(CBankStateBlock& output);
    virtual unsigned long CreateCRC();

    CStateBlock State;       // +0x04

protected:
    virtual void Set(const CBankStateBlock* source);
};
FABLE_STATIC_ASSERT(offsetof(CWADBankStateBlock, State) == 0x4);
FABLE_STATIC_ASSERT(sizeof(CWADBankStateBlock) == 0x8);
