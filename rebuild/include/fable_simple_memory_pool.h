#pragma once
#include <stddef.h>
#include "fable_base_class.h"
#include "fable_resource_list.h"

// Ego_r.pdb layout and retail A5C570..A5C993. The generated engine snapshot
// flattens these bases; native providers share the actual polymorphic types.
class CSimpleMemoryPool : public CBaseClassNonCopyable
{
public:
    enum EAreaStatus { STATUS_FREE=97, STATUS_ALLOCATED=103 };
    struct CArea
    {
        unsigned long Size;
        CArea* NextFreeArea;
        EAreaStatus Status;
        void* GetDataPointer(){return (unsigned char*)this+16;}
    };
    class CMemoryBlock : public CIVCountedPointeeBase
    {
    public:
        explicit CMemoryBlock(unsigned long size);
        virtual ~CMemoryBlock();
        void* BlockData;
        bool OwnsData;
        unsigned long BlockSize;
        CMemoryBlock* NextBlock;
    };
    CSimpleMemoryPool(unsigned long capacity,unsigned long growth);
    virtual ~CSimpleMemoryPool();
    void SetCapacity(unsigned long capacity);
    unsigned long CalculateMemoryAllocatedByPool() const;
    void FreeAll();
    void* Allocate(unsigned long size);
    CMemoryBlock* FirstMemoryBlock;
    unsigned long Capacity,GrowthGranularity;
    CArea Root;
private:
    static unsigned long Align(unsigned long size){return ((size+15)/16)*16;}
    void CreateNewMemoryBlock(unsigned long size);
    __forceinline void InsertFreeArea(CArea* area)
    {
        area->Status=STATUS_FREE;
        area->NextFreeArea=Root.NextFreeArea;
        Root.NextFreeArea=area;
    }
    __forceinline CArea* CreateArea(void* data,unsigned long size)
    {
        CArea* area=(CArea*)data;
        area->Size=size-16;
        InsertFreeArea(area);
        return area;
    }
    __forceinline void ClearMemoryBlocks()
    {
        CMemoryBlock* block=FirstMemoryBlock;
        if(block)
        {
            if(block->OwnsData)
            {
                do
                {
                    CMemoryBlock* next=block->NextBlock;
                    delete block;
                    block=next;
                }while(block);
            }
            else delete block;
            FirstMemoryBlock=0;
        }
    }
};
FABLE_STATIC_ASSERT(sizeof(CSimpleMemoryPool::CArea)==12);
FABLE_STATIC_ASSERT(sizeof(CSimpleMemoryPool::CMemoryBlock)==24);
FABLE_STATIC_ASSERT(sizeof(CSimpleMemoryPool)==28);
FABLE_STATIC_ASSERT(offsetof(CSimpleMemoryPool,FirstMemoryBlock)==4);
FABLE_STATIC_ASSERT(offsetof(CSimpleMemoryPool,Root)==16);
