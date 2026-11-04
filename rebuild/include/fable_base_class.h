#pragma once

#include "rebuild_abi.h"

// Retail root class: RTTI .?AVCBaseClass@@, vtable 0x01231710 (one slot, the scalar
// deleting destructor 0x004371F0). Ctor 0x0099A2D0 and dtor 0x0099A2E0 are out of
// line; the CBaseClass/CInitBaseClass/CBaseClassNonCopyable translation unit
// (0x0099A2D0..0x0099A4D0) inlines them into its siblings.
class CBaseClass
{
public:
    CBaseClass();
    virtual ~CBaseClass();
};

// RTTI .?AVCBaseClassNonCopyable@@, vtable 0x0129A7C4 (scalar deleting destructor
// 0x0099A470); ctor 0x0099A2F0, dtor 0x0099A300.
class CBaseClassNonCopyable : public CBaseClass
{
public:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();

private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
