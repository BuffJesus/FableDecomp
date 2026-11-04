#pragma once
#include "fable_base_class.h"

class CBaseObject;
// FableWin/Ego_r CBaseObjectPointer: 8 bytes, exact member types/names.
struct CBaseObjectPointer
{
    CBaseObject* object;
    unsigned long refcount;
};
class CBaseObject : public CBaseClass
{
public:
    // FableWin ?Validate@CBaseObject@@UBEXXZ: vtable slot 1 (retail 0x0099A400;
    // CClass vtable 0x0129CF60 slot 1).
    virtual void Validate() const;
    CBaseObjectPointer* IntelligentPointer;
};
class CBaseIntelligentPointer
{
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject* item);
private:
    friend struct CBaseIntelligentPointerTestAccess;
    void ClearData() const;
    mutable CBaseObjectPointer* PData;
};
FABLE_STATIC_ASSERT(sizeof(CBaseObjectPointer)==8);
FABLE_STATIC_ASSERT(sizeof(CBaseObject)==8);
FABLE_STATIC_ASSERT(sizeof(CBaseIntelligentPointer)==8);

// FableWin CIntelligentPointer<T>: primary 8-byte CBaseIntelligentPointer.
// Get/GetPointer inline to GetPItem; the pointed-to primary ancestry is +0.
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer
{
public:
    T* Get() const { return GetPointer(); }
private:
    T* GetPointer() const { return static_cast<T*>(GetPItem()); }
};
