#pragma once

#include "rebuild_abi.h"
#include "fable_resource_list.h"

// Moved verbatim from fable_game.h so CDefinitionManager's RecompiledDefs map
// can size its nodes without the whole game header.

// Ref-counted base of every def object (retail RTTI: CDefPointeeBase :
// CResource : CIVCountedPointeeBase; vtable 0x01230BA0, 0x1C bytes, no
// members of its own). The lock count is CIVCountedPointeeBase's IVRefCount
// (+4, 1 after construction) and slot 1 (DeleteCIVCountedPointeeBase
// 0x0042D930) runs when an unlock takes it to zero.
class CDefPointeeBase : public CResource
{
public:
    void Unlock() const
    {
        if (--IVRefCount == 0)
            DeleteCIVCountedPointeeBase();
    }

    // FableWin ?Lock@CDefPointeeBase@@QBEXXZ: the +4 count bump retail's
    // CDefinitionManager::GetPDefFromGlobalIndex (0x009AD9E0) inlines.
    void Lock() const
    {
        ++IVRefCount;
    }
};
FABLE_STATIC_ASSERT(sizeof(CDefPointeeBase) == 0x1C);

template <class T>
class CDefPointer
{
public:
    CDefPointer()
        : Object(0)
    {
    }

    // FableWin ??0?$CDefPointer@VCDefClassBase@@@@QAE@PAVCDefClassBase@@@Z; inline
    // in retail. It takes over the new object's initial reference without
    // locking: InstantiateDef 0x009ADC90 holds a freshly allocated def this
    // way, locks it once (the CInstantiatedDefInfo copy) and unlocks it once.
    explicit CDefPointer(T* object)
        : Object((void*)object)
    {
    }

    // FableWin ??0?$CDefPointer@...@@QAE@ABV0@@Z; inline in retail
    // (CMainGameComponent::Run 0x004189C2 copies CRegion::PDef through the
    // inline CRegion::GetPRegionDef): take the pointer, then lock it.
    __forceinline CDefPointer(const CDefPointer& other)
        : Object(other.Object)
    {
        if (Object)
            static_cast<CDefPointeeBase*>(Object)->Lock();
    }

    // Inlined in retail even in /Os TUs (~CNewFrontendGameComponent
    // 0x0042F50B), where CCountedPointer's larger dtor stays out of line;
    // VC7.1 /Os only inlines it when forced (plain/inline/Release-wrapped
    // forms all stay out of line). Unlock the pointee, then clear it.
    __forceinline ~CDefPointer()
    {
        if (Object)
        {
            static_cast<CDefPointeeBase*>(Object)->Unlock();
            Object = 0;
        }
    }

    // Ego_r ?GetRefCount@CDefPointer@...@@QBEKXZ (inline): the pointee's lock
    // count, zero for a null pointer.
    unsigned long GetRefCount() const
    {
        if (Object == 0)
            return 0;
        return static_cast<CDefPointeeBase*>(Object)->IVRefCount;
    }

    // FableWin ?Release@CDefPointer@...@@QAEXXZ; inline in retail (same
    // body as the dtor).
    __forceinline void Release()
    {
        if (Object)
        {
            static_cast<CDefPointeeBase*>(Object)->Unlock();
            Object = 0;
        }
    }

    // FableWin ?Reset@CDefPointer@...@@QAEXPB...@Z; inline in retail
    // (CNewFrontendGameComponent::Shutdown 0x0042EBB6 inlines Reset(0):
    // Release, then the unconditional Object store). Like CDefPointer(T*) it
    // takes over the object's reference without locking it:
    // CDefinitionManager::LoadDef 0x009AD6E0 resets PDef to a new def with
    // no lock.
    __forceinline void Reset(T* object)
    {
        Release();
        Object = (void*)object;
    }

    // Conversion from a def pointer of another def type (CDefPointer<T> to
    // CDefPointer<const T>); inline in retail, same body as the copy ctor:
    // CDefinitionManager::GetPDefFromClassIndex 0x009ADE10 returns an
    // instantiated def's CDefPointer<CDefClassBase> as a <const CDefClassBase>.
    template <class U>
    __forceinline CDefPointer(const CDefPointer<U>& other)
        : Object(other.Object)
    {
        if (Object)
            static_cast<CDefPointeeBase*>(Object)->Lock();
    }

    // FableWin's same-type operator= (CCombatTypeDef const specialization:
    // thunk 017EB70B -> body 01DA50D0). A templated assignment does not
    // suppress C++'s implicit copy assignment; spell out the engine's lock
    // and release behavior for assignments of the same def-pointer type.
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object)
        {
            Release();
            Object = other.Object;
            if (Object)
                static_cast<CDefPointeeBase*>(Object)->Lock();
        }
        return *this;
    }

    // Assignment from a def pointer of any def type (the object is shared, so
    // it takes a lock); inline in retail. GetDef<T> 0x004300FB assigns its
    // CDefPointer<const CDefClassBase> into the caller's CDefPointer<const T>:
    // nothing when both hold the same def, else Release (unlock, clear),
    // then store and lock.
    template <class U>
    __forceinline CDefPointer& operator=(const CDefPointer<U>& other)
    {
        if (Object != other.Object)
        {
            Release();
            Object = other.Object;
            if (Object)
                static_cast<CDefPointeeBase*>(Object)->Lock();
        }
        return *this;
    }

    // FableWin ??C?$CDefPointer@...@@QBEPB...@XZ; inline in retail.
    // FableWin operator*: debug null assertion is compiled out in retail.
    T& operator*() const { return *static_cast<T*>(Object); }

    T* operator->() const
    {
        return static_cast<T*>(Object);
    }

    // FableWin ?Get@?$CDefPointer@...@@QBEPB...@XZ; inline in retail.
    T* Get() const
    {
        return static_cast<T*>(Object);
    }

    void* Object;
};
