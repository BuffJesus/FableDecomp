#pragma once
#include "fable_small_vector.h"
#include "fable_string.h"
// Ego_r member names; retail EntrySetUpdate reads +0x14. Retail allocates 0x18 bytes
// (CBankFile::ReadEntryList 009CFBC0: operator new(0x18), inline constructor below) and
// its destructor (CloseBankFileImpl 009CD850) releases only the info buffer and
// filenames: Ego_r's trailing CCountedPointer<CBankStateBlock> StateBlock is not in
// the retail build.
class CBankFileEntryUpdateData {
public:
    CBankFileEntryUpdateData() : StateBlockCRC(0), InfoSize(0), InfoBuffer(0), RequiresUpdate(false), Exists(true) {}
    void ReadInfo(unsigned long,void*,unsigned long) const;
    unsigned long StateBlockCRC;
    unsigned long InfoSize;
    char* InfoBuffer;
    CSmallVector<CCharString,8> Filenames;
    bool RequiresUpdate;
    bool Exists;
};
FABLE_STATIC_ASSERT(sizeof(CBankFileEntryUpdateData)==0x18);
FABLE_STATIC_ASSERT(offsetof(CBankFileEntryUpdateData,RequiresUpdate)==0x14);
