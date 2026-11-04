#pragma once

// CFrontEndDef (the FRONT_END def class): retail sizeof 0x54 (operator
// new(0x54) in Alloc 0x00433080; GetSizeofClass 0x00431E30), vtable
// 0x01230FBC. Ego_r's is 0x5C: its CParentDefClassBase and vector are each
// 4 bytes larger. vAttractModeMovie (a 12-byte STLport vector) sits at +0x3C
// (CNewFrontendGameComponent::Run 0x0042EC7C reads its begin/end at
// +0x3C/+0x40).

#include "fable_def_class_base.h"
#include "fable_string.h"
#include "fable_stlport_vector.h"

class CFrontEndDef : public CParentDefClassBase
{
public:
    // FableWin ?Alloc@CFrontEndDef@@SIPAVCDefClassBase@@XZ, retail 0x00433080
    // (registered by CNewFrontendGameComponent::CompileDefsPlatformSpecific).
    static CDefClassBase* FABLE_FASTCALL Alloc();

    // FableWin 0x00432E60: the out-of-line assignment target of Copy.
    CFrontEndDef& operator=(const CFrontEndDef& other);

    virtual void Transfer(CPersistContext& context);   // [18] 0x004331C0
    virtual void Copy(const CDefClassBase* other);     // [19] 0x00432E50
    virtual unsigned long GetSizeofClass() const;      // [20] 0x00431E30

    _STL::vector<CCharString> vAttractModeMovie;       // +0x3C
    long ErrorMessageBackgroundGraphic;                // +0x48
    long ButtonABigGraphic;                            // +0x4C
    long ButtonBBigGraphic;                            // +0x50
};
