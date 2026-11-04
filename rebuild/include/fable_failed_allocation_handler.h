#pragma once

// A CHandleBasedMemoryPool failure hook (FableWin ?SetFailureHandler@...).
// Retail vtable 0x0129B874 (RTTI .?AVCFailedAllocationHandler@@): slot 0 its
// scalar deleting destructor, slot 1 _purecall. No members; ctor and dtor are
// inline in retail (~CResourceList 0x009FC5C0 ends by restoring this vptr).
class CFailedAllocationHandler
{
public:
    virtual ~CFailedAllocationHandler()
    {
    }
    virtual bool HandleFailedAllocation(unsigned long size) = 0;
};
