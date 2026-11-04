#pragma once

// Retail bank checksum cache: 0x18 bytes. The donor's 0x1C layout used a
// larger map node; the retail constructor allocates a 0x34-byte header node.
#include "fable_string.h"
#include "fable_date_and_time.h"
#include "fable_bank_owned_map.h"

class CChecksumCacheFile
{
public:
    ~CChecksumCacheFile();
    void Close();
    void Open(const CWideString& filename);
    unsigned long GetChecksum(const CWideString& filename);
    struct CEntry { CDateAndTime TimeStamp; unsigned long Checksum; };

    // Fixture-only default for authored bank boundaries. Retail bank
    // construction passes an explicit requested buffer size.
    CChecksumCacheFile() : BufferSize(0), NeedsSaving(false) {}
    explicit CChecksumCacheFile(long requestedBufferSize);
    CChecksumCacheFile(const CWideString& filename,long requestedBufferSize);

    CWideString Filename;
    FableBankOwnedMap<CWideString, CEntry> Entries;
    long BufferSize;
    bool NeedsSaving;
    unsigned char Padding15[3];
private:
    void Save();
};
FABLE_STATIC_ASSERT(sizeof(CChecksumCacheFile) == 0x18);

FABLE_STATIC_ASSERT(sizeof(CChecksumCacheFile::CEntry) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CChecksumCacheFile::CEntry, Checksum) == 0x1c);

// Retail's cache-tree decrement returns the iterator receiver (0x00A60890).
// Keep the node step in this typed operator instead of the void base helper.
template<> inline _STL::_Rb_tree_iterator<_STL::map<CWideString,CChecksumCacheFile::CEntry>::value_type>&
_STL::_Rb_tree_iterator<_STL::map<CWideString,CChecksumCacheFile::CEntry>::value_type>::operator--() {
    _M_node=_STL::_Rb_global_inst::_M_decrement(_M_node);
    return *this;
}
