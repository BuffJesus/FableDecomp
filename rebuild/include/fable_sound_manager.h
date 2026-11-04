#pragma once

#include "fable_base_class.h"
#include "fable_string.h"

namespace LHA
{
    // RTTI-named; the one virtual is the vtable at 0x122ED84 (pure).
    class CBinaryDataWriter
    {
    public:
        virtual bool Write(const void* data, long size) = 0;
    };
}

// RTTI: CSoundManagerLH : CASoundManager : CBaseClassNonCopyable : CBaseClass.
// FableWin has no CASoundManager, so unknown slot names are retained. Known
// signatures come from retail callers and CSoundManagerLH implementations.
class CASoundManager : public CBaseClassNonCopyable
{
public:
    virtual void UnknownSlot01();
    virtual void UnknownSlot02();
    virtual void UnknownSlot03();
    virtual void UnknownSlot04();
    virtual void UnknownSlot05();
    virtual void UnknownSlot06();
    virtual void UnknownSlot07();
    virtual void UnknownSlot08();
    virtual void UnknownSlot09();
    virtual void UnknownSlot10();
    virtual void UnknownSlot11();
    virtual void UnknownSlot12();
    virtual void UnknownSlot13();
    virtual void UnknownSlot14();
    virtual void UnknownSlot15();
    virtual void UnknownSlot16();
    virtual void UnknownSlot17();
    // +0x48. Takes one dword (CSoundManagerLH's 0xA3E520 ends `ret 4`);
    // CNewFrontendGameComponent::Shutdown 0x0042EBB6 passes 0.
    virtual void UnknownSlot18(long unknown);
    virtual void UnknownSlot19();
    virtual void UnknownSlot20();
    virtual void UnknownSlot21();
    virtual void UnknownSlot22();
    virtual void UnknownSlot23();
    virtual void UnknownSlot24();
    virtual void UnknownSlot25();
    virtual void UnknownSlot26();
    virtual void UnknownSlot27();
    virtual void UnknownSlot28();
    virtual void UnknownSlot29();
    virtual void UnknownSlot30();
    virtual void UnknownSlot31();
    virtual void UnknownSlot32();
    virtual void UnknownSlot33();
    virtual void UnknownSlot34();
    virtual void UnknownSlot35();
    virtual void UnknownSlot36();
    virtual void UnknownSlot37();
    virtual void UnknownSlot38();
    virtual void UnknownSlot39();
    virtual void UnknownSlot40();
    virtual void UnknownSlot41();
    virtual void UnknownSlot42();
    virtual void UnknownSlot43();
    virtual void UnknownSlot44();
    virtual void UnknownSlot45();
    // +0xB8 / +0xBC: activation's group 0x8000, saved x87 float, and zero
    // transition; CSoundManagerLH 00A3E9D0 / 00A3EA20 return with RET 12 / 4.
    virtual void UnknownSlot46(unsigned long group, float value, float transition);
    virtual float UnknownSlot47(unsigned long group);
    virtual void UnknownSlot48();
    // +0xC4: 00A3E480 forwards an output pointer and returns a bool (RET 4).
    virtual bool UnknownSlot49(float& offset);
    virtual void UnknownSlot50();
    // +0xCC. CSoundManagerLH (0xA3E800) hands the path and writer to its
    // lip-sync generator; the .lug -> .met conversion.
    virtual void GenerateLipSyncData(
        const CWideString& lugPathname,
        LHA::CBinaryDataWriter& writer);

    // 009915E0 toggles the byte at +0x25 and calls slot 18(0) on disable.
    // The inherited CWorld::SetMiniMapEnabled label is contradicted by this
    // sound-manager receiver in ActivateWindow and WinProcedure.
    void UnknownSetEnabled_009915e0(bool enabled);

    // Non-virtual CASoundManager members (names not recovered; FableWin has
    // no CASoundManager). 0x00991750: if the bool at +0x24 is set, clear it
    // and call slot 16, then call slot 2 on the object at +4 of every 0x24-byte
    // entry of the vector at +0x30. 0x009918F0: erase that whole vector.
    // Caller: CNewFrontendGameComponent::Shutdown 0x0042EBB6.
    void UnknownUpdateEntries_00991750();
    void UnknownClearEntries_009918F0();
};
