#pragma once
#include "fable_base_class.h"
#include "fable_iv_counted_pointer.h"
#include "fable_stlport_tree.h"
class CEnginePrimitiveHandle;

// Ego_r.pdb: 0x0C, intrusive pointee base followed by the primitive type.
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase
{
public:
    virtual void CollectStats(_STL::map<unsigned long,unsigned long>&,long,unsigned long) const;
    virtual void NotifyReleased(const CEnginePrimitiveHandle&);
    virtual bool WasRendered() const;
    long Type;
};

class CEnginePrimitiveHandle : public CBaseClass
{
public:
    virtual ~CEnginePrimitiveHandle()
    {
        if (Data.data_ && Data.data_->IVRefCount == 1)
            Data.data_->NotifyReleased(*this);
    }
    __forceinline void Clear()
    {
        if (Data.data_ && Data.data_->IVRefCount == 1)
            Data.data_->NotifyReleased(*this);
        Data.Release();
        Data.data_=0;
    }
    __forceinline void SetData(CEnginePersistentPrimitiveBase* value)
    {
        Clear();
        Data.Release();
        Data.data_=value;
    }
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePersistentPrimitiveBase)==12);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle)==8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveHandle,Data)==4);
