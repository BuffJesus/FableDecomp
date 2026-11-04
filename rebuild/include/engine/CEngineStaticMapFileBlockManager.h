#pragma once
#include <stddef.h>
#include "fable_counted_pointer.h"
#include "fable_stlport_tree.h"
class CBankFileAsyncEntry;
class CBankFileAsyncData;
class CEngineStaticMapFileBlockManager
{
public:
    ~CEngineStaticMapFileBlockManager();
    void Reset();
    void SetBankFileEntry(CCountedPointer<CBankFileAsyncEntry>);
    CCountedPointer<CBankFileAsyncEntry> GetBankFileEntry() const { return AsyncBankEntry; }
    struct CFileBlock {
        long RefCount;
        bool LowPriority;
        CCountedPointer<CBankFileAsyncData> LoadingData;
    };
    void AddRequest(long blockId, unsigned long blockSize, bool immediate);
    bool IsBlockLoaded(long blockId);
    unsigned char* GetBlockData(long blockId, unsigned long blockSize);
    void ReleaseRequest(long);
    void DeleteUnusedBlocks();
    CCountedPointer<CBankFileAsyncEntry> AsyncBankEntry;
    _STL::map<long,CFileBlock> Blocks;
};

typedef _STL::map<long,CEngineStaticMapFileBlockManager::CFileBlock>::_Rep_type CStaticMapBlockTree;
namespace _STL {
template<> CStaticMapBlockTree::iterator CStaticMapBlockTree::find(const long& key);
}
FABLE_STATIC_ASSERT(sizeof(CEngineStaticMapFileBlockManager::CFileBlock)==0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineStaticMapFileBlockManager,Blocks)==8);
FABLE_STATIC_ASSERT(sizeof(CEngineStaticMapFileBlockManager)==0x14);

