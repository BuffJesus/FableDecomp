#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

// Shared ownership block behind every CCountedPointer (Ego_r.pdb layout).
class CCPPointerInfo
{
public:
    CCPPointerInfo(void (FABLE_FASTCALL *deleteFunc)(void*), void* data)
        : RefCount(1), DeleteFunc(deleteFunc), Data(data)
    {
    }

    unsigned long RefCount;
    void (FABLE_FASTCALL *DeleteFunc)(void*);
    void* Data;
};

FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo) == 0x0C);

// Reset is defined (and explicitly instantiated) in each retail
// instantiation's source, so fixtures link the reconstructed copy.
template<typename T>
class CCountedPointer
{
public:
    CCountedPointer()
        : Data(0),
          Info(0)
    {
    }

    // Inline in retail; /Os TUs keep it out of line as a per-T COMDAT
    // (CCountedPointer<const CFontBank>: 0x0042F948, called from
    // ~CNewFrontendGameComponent 0x0042F50B).
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }

    // Out of line in retail (CCountedPointer<CGraphicDataBank> 0x0041BA30,
    // <CMeshDataBank> 0x00428905: copy both fields, then ++Info->RefCount).
    CCountedPointer(const CCountedPointer& other);
    // A CCountedPointer<const T> sharing a CCountedPointer<T> (FableWin e.g.
    // ??$?0VC3DSkeletalMorph@@@?$CCountedPointer@$$CBVC3DSkeletalMorph@@@@QAE@ABV?$CCountedPointer@VC3DSkeletalMorph@@@@@Z;
    // inline body in fable_counted_pointer_inline.h).
    template<typename U>
    CCountedPointer(const CCountedPointer<U>& other);

    // FableWin ??0?$CCountedPointer@...@@QAE@PAV...@Z: out of line in retail
    // (per-T COMDAT, e.g. <NParticleEngine::CAEnvironmentInteraction>
    // 0x0041956C: store Data, then a fresh CCPPointerInfo when non-null).
    CCountedPointer(T* data);

    void Reset(T* data);

    // FableWin ?ReleaseData@...@@QAEXXZ: out of line in retail (drop the
    // reference, then clear Data and Info; e.g. <CIEngine> 0x0042F974).
    void ReleaseData();

    // PDB public sharing helper; preserves identity when control blocks match.
    void ShareData(T* data, CCPPointerInfo* info);

    // FableWin ??$SetStaticCast@...: share a CCountedPointer<U>'s data as T
    // (inline bodies in fable_counted_pointer_inline.h).
    template<typename U>
    void SetStaticCast(const CCountedPointer<U>& other);

    // FableWin ?IsNull@...@@QBE_NXZ; inline in retail.
    bool IsNull() const
    {
        return Data == 0;
    }

    // FableWin ?Get@...@@QBEPAV...@XZ; inline in retail.
    T* Get() const
    {
        return Data;
    }

    // FableWin ??C?$CCountedPointer@...@@QBEPAV...@XZ; inline in retail
    // (NProgressDisplay::IsActive 0x009E9FB0 dispatches through it).
    T* operator->() const
    {
        return Data;
    }

    // FableWin ??D?$CCountedPointer@...@@QBEAAV...@XZ; inline in retail
    // (NParticleEngine::GetParticleEmitterDatabase 0x00AEAAE0).
    T& operator*() const
    {
        return *Data;
    }

    // Out of line in retail (per-T COMDATs): CCountedPointer<CGraphicDataBank>
    // 0x004194BA and CCountedPointer<const CFontBank> from CCountedPointer<CFontBank>
    // 0x0042FE34 (FableWin ??4?$CCountedPointer@...@@QAEAAV0@ABV0@@Z and
    // ??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@QAEAAV0@ABV?$CCountedPointer@VCFontBank@@@@@Z).
    CCountedPointer& operator=(const CCountedPointer& other);
    template<typename U>
    CCountedPointer& operator=(const CCountedPointer<U>& other);

    // FableWin's lower-case alias (?reset@...); retail inlines it to Reset.
    void reset(T* data)
    {
        Reset(data);
    }

    T* Data;
    CCPPointerInfo* Info;

private:
    static void FABLE_FASTCALL DeleteData(void* data)
    {
        delete static_cast<T*>(data);
    }

    void Release()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
