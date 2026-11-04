#pragma once

// Retail bank constructors install a distinct vtable in each state block.
// The donor-generated layout records the bytes but not that constructor ABI.
#include "rebuild_abi.h"

// The donor PDB records CBankStateBlock as a four-byte polymorphic base.
// GetDefault receives that base reference inside a concrete font state block.
class CBankStateBlock
{
public:
    // Retail base vtable 0x0129C82C: deleting destructor followed by three
    // pure virtual slots. Derived destruction must restore this base vtable.
    virtual ~CBankStateBlock() {}
    virtual void GetDefault(CBankStateBlock& output) = 0;
    virtual unsigned long CreateCRC() = 0;
protected:
    virtual void Set(const CBankStateBlock* source) = 0;
};
FABLE_STATIC_ASSERT(sizeof(CBankStateBlock) == 4);

class CFontBankStateBlock : public CBankStateBlock
{
public:
    virtual ~CFontBankStateBlock();
    virtual void GetDefault(CBankStateBlock& output);
    virtual unsigned long CreateCRC();
    unsigned char State[0x0c];
protected:
    virtual void Set(const CBankStateBlock* source);
};
FABLE_STATIC_ASSERT(sizeof(CFontBankStateBlock) == 0x10);

class CStreamingFontBankStateBlock : public CBankStateBlock
{
public:
    virtual ~CStreamingFontBankStateBlock();
    virtual void GetDefault(CBankStateBlock& output);
    virtual unsigned long CreateCRC();
    unsigned char State[0x10];
protected:
    virtual void Set(const CBankStateBlock* source);
};
FABLE_STATIC_ASSERT(sizeof(CStreamingFontBankStateBlock) == 0x14);
