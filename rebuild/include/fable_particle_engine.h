#pragma once

// NParticleEngine's free functions and the environment-interaction hook as
// CMainGameComponent reaches them (FableWin.pdb names, retail addresses from
// CMainGameComponent::InitParticleEngine 0x004174F1 / Shutdown 0x004175E5).
//
// Retail RTTI: CParticleEnvironmentInteraction :
// NParticleEngine::CAEnvironmentInteraction; vtable 0x0122ED8C (7 slots,
// donor 0x04022198): 0 ??_E, 1 IsParticleCollidingAt, 2 IsWindActive,
// 3 GetWindVelocity, 4 GetGravityVector, 5 GetVectorParam, 6 GetCameraPos.
// All seven slots are declared (FableWin signatures; parameter names are
// descriptive). The constructors are inline in retail (InitParticleEngine
// stores just the vptr and PWorld).

#include <stddef.h>
#include "fable_2d_vector.h"
#include "fable_counted_pointer.h"
#include "fable_string.h"
#include "fable_symbol_map.h"
#include "fable_vector_map.h"
#include "rebuild_abi.h"

#include "fable_game_text_bank.h"

class CWorld;
class CGraphicDataBank;
class CMeshDataBank;
class CSymbolMap;

namespace NParticleEngine
{
    class CParticleEmitter;
    // Ego_r.pdb: vptr only (4 bytes).
    class CAEnvironmentInteraction
    {
    public:
        // FableWin ??0CAEnvironmentInteraction@NParticleEngine@@QAE@XZ; inline.
        CAEnvironmentInteraction() {}
        virtual ~CAEnvironmentInteraction();
        virtual bool IsParticleCollidingAt(
            const C3DVector& from,
            const C3DVector& to,
            bool flag,
            C3DVector& collisionPos,
            C3DVector& collisionNormal,
            bool& collided) const = 0;
        virtual bool IsWindActive() const;
        virtual C3DVector GetWindVelocity() const = 0;
        virtual C3DVector GetGravityVector() const;
        virtual C3DVector GetVectorParam(const CCharString& name) const;
        virtual void GetCameraPos(C3DVector& position) const = 0;
    };

    // RTTI: NParticleEngine::CParticleDataBank : CBankFileAsync, CResourceBank
    // (Ego_r.pdb). Retail sizeof 0x234 (OpenBank's `new`); the CResourceBank
    // base sits at +0x164 (ctor 0x00AF1670).
    class CParticleDataBank : public CBankFileAsync, public CResourceBank
    {
    public:
        CParticleDataBank();
        virtual ~CParticleDataBank();
        virtual void Close();
    protected:
        virtual unsigned long GetDataFormatVersion();
        virtual void NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&);
        virtual bool IsDiskFile(unsigned long, unsigned long);
    public:


        fable_u8 particleDataBank_[0x234 - 0x164 - sizeof(CResourceBank)];
    };

    class CParticleEmitterDatabase
    {
    public:
        // Out of line in retail (0x00AE8BF0; Initialise 0x00AEBE20 calls it).
        CParticleEmitterDatabase();
        ~CParticleEmitterDatabase();

        // ?OpenBank@CParticleEmitterDatabase@NParticleEngine@@QAEXABVCCharString@@@Z
        // (retail 0x00AE8980).
        void OpenBank(const CCharString& bankName);

        // Ego_r.pdb member names; retail layout 0x4C (Ego_r 0x58): the 0x24-byte CSymbolMap (ctor 0x009E5200), the
        // UsingSymbolMap flag, the 16-byte emitter CVectorMap, the bank, the next handle and the two file names.
        CSymbolMap& GetSymbolMap()
        {
            return SymbolMap;
        }

        CSymbolMap SymbolMap;                                     // +0x00
        bool UsingSymbolMap;                                      // +0x24
        CVectorMap<long, CCountedPointer<CParticleEmitter>,
                   CKeyPairCompareLess<long, CCountedPointer<CParticleEmitter> > > Emitters;  // +0x28
        CCountedPointer<CParticleDataBank> EmitterBank;           // +0x38
        long CurrentHandle;                                       // +0x40
        CCharString ParFilename;                                  // +0x44
        CCharString HeaderFilename;                               // +0x48
    };
    FABLE_STATIC_ASSERT(offsetof(CParticleEmitterDatabase, Emitters) == 0x28);
    FABLE_STATIC_ASSERT(offsetof(CParticleEmitterDatabase, CurrentHandle) == 0x40);
    FABLE_STATIC_ASSERT(sizeof(CParticleEmitterDatabase) == 0x4c);

    // Ego_r.pdb names; retail sizeof from Initialise's `new` (0x1018 / 0x10),
    // constructors out of line (0x00AEBA90 / 0x00AF2970).
    class CParticleEmitterManager
    {
    public:
        CParticleEmitterManager();
        ~CParticleEmitterManager();

        fable_u8 data_[0x1018];
    };

    class CPSCManager
    {
    public:
        CPSCManager();
        ~CPSCManager();

        fable_u8 data_[0x10];
    };

    // Ego_r.pdb namespace globals (?PEmitterDatabase@NParticleEngine@@3V...@A
    // etc.). Retail addresses: PGraphicDataBank 0x013D2E08,
    // PEmitterDatabase 0x013D2E14, PEmitterManager 0x013D2E1C,
    // PPSCManager 0x013D2E24, PEnvironmentInteraction 0x013D2E2C.
    extern CCountedPointer<CParticleEmitterDatabase> PEmitterDatabase;
    extern CCountedPointer<CParticleEmitterManager> PEmitterManager;
    extern CCountedPointer<CPSCManager> PPSCManager;
    extern CCountedPointer<CAEnvironmentInteraction> PEnvironmentInteraction;

    // ?Initialise@NParticleEngine@@YIXXZ (retail 0x00AEBE20),
    // ?Uninitialise@NParticleEngine@@YIXXZ (retail 0x00AEBEA0),
    // ?SetEnvironmentInteraction@NParticleEngine@@YIXV?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@@Z
    // (retail 0x00AEAB20; the counted pointer is passed by value) and
    // ?GetParticleEmitterDatabase@NParticleEngine@@YIAAVCParticleEmitterDatabase@1@XZ
    // (retail 0x00AEAAE0).
    void FABLE_FASTCALL Initialise();
    void FABLE_FASTCALL Uninitialise();
    void FABLE_FASTCALL SetEnvironmentInteraction(
        CCountedPointer<CAEnvironmentInteraction> environmentInteraction);
    CParticleEmitterDatabase& FABLE_FASTCALL GetParticleEmitterDatabase();
    // ?SetGraphicBank@NParticleEngine@@YIXABVCGraphicDataBank@@@Z (retail
    // 0x00AEAA80: `mov [0x013D2E08], ecx; ret`); caller
    // CNewFrontendGameComponent::Init2 0x0042E98F.
    void FABLE_FASTCALL SetGraphicBank(const CGraphicDataBank& bank);
    // Bank pointers set by SetGraphicBank / SetMeshBank (ParticleBankSetters_00aeaa80.cpp; retail 0x013D2E08 /
    // 0x013D2E0C).
    extern const CGraphicDataBank* PGraphicDataBank;
    extern const CMeshDataBank* PMeshDataBank;
    // Ego_r particle_engine.cpp accessors (retail 0x00AEA9C0-0x00AEAB10, in Ego_r order).
    bool FABLE_FASTCALL IsRenderingEnabled();
    void FABLE_FASTCALL SetTickRate(long tickRate);
    long FABLE_FASTCALL GetTickRate();
    float FABLE_FASTCALL GetOneOverTickRate();
    float FABLE_FASTCALL GetLODDistance();
    float FABLE_FASTCALL GetFadeOutStartDistance();
    float FABLE_FASTCALL GetFadeInEndDistance();
    float FABLE_FASTCALL GetFadeInStartDistance();
    float FABLE_FASTCALL GetMaxDrawDistance();
    float FABLE_FASTCALL GetMaxSpawnDistance();
    void FABLE_FASTCALL SetMeshGraphicBank(const CGraphicDataBank& bank);
    void FABLE_FASTCALL SetMeshBank(const CMeshDataBank& bank);
    const CGraphicDataBank& FABLE_FASTCALL GetGraphicBank();
    const CMeshDataBank& FABLE_FASTCALL GetMeshBank();
    void FABLE_FASTCALL SetRandomSeed(unsigned long seed);
    unsigned long& FABLE_FASTCALL GetRandomSeed();
    CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager();
    CPSCManager& FABLE_FASTCALL GetPSCManager();
    CAEnvironmentInteraction& FABLE_FASTCALL GetEnvironmentInteraction();
}

// lib_counted_pointer.hpp's ReleaseData is inline; particle_engine.cpp's
// Uninitialise (0x00AEBEA0) inlines it for its three singletons, while other
// retail TUs keep out-of-line copies (see fable_counted_pointer.h).
template<>
inline void CCountedPointer<NParticleEngine::CParticleEmitterManager>::ReleaseData()
{
    Release();
    Info = 0;
    Data = 0;
}

template<>
inline void CCountedPointer<NParticleEngine::CParticleEmitterDatabase>::ReleaseData()
{
    Release();
    Info = 0;
    Data = 0;
}

// Inlined by SetEnvironmentInteraction (0x00AEAB20): take the other side's
// fields, and only when the blocks differ release ours and add a reference.
template<>
inline CCountedPointer<NParticleEngine::CAEnvironmentInteraction>&
CCountedPointer<NParticleEngine::CAEnvironmentInteraction>::operator=(
    const CCountedPointer& other)
{
    NParticleEngine::CAEnvironmentInteraction* data = other.Data;
    CCPPointerInfo* info = other.Info;
    if (Info != info)
    {
        Release();
        Data = data;
        Info = info;
        if (Info)
            ++Info->RefCount;
    }
    return *this;
}

template<>
inline void CCountedPointer<NParticleEngine::CPSCManager>::ReleaseData()
{
    Release();
    Info = 0;
    Data = 0;
}

// Ego_r.pdb layout (0x24). The engine's C3DVector packs to byte alignment
// here: CameraPos follows WindActive at +0x15 (Ego_r.pdb, and retail
// GetCameraPos 0x0049BDC0 reads +0x15/+0x19/+0x1D), then 3 bytes of tail
// padding. Packed so the model keeps that offset and the 0x24 size.
#pragma pack(push, 1)
class CParticleEnvironmentInteraction : public NParticleEngine::CAEnvironmentInteraction
{
public:
    // FableWin ??0CParticleEnvironmentInteraction@@QAE@ABVCWorld@@@Z; inline.
    explicit CParticleEnvironmentInteraction(const CWorld& world)
        : PWorld(&world)
    {
    }

    virtual ~CParticleEnvironmentInteraction();
    virtual bool IsParticleCollidingAt(
        const C3DVector& from,
        const C3DVector& to,
        bool flag,
        C3DVector& collisionPos,
        C3DVector& collisionNormal,
        bool& collided) const;
    virtual C3DVector GetWindVelocity() const;
    virtual void GetCameraPos(C3DVector& position) const;

    const CWorld* PWorld;           // +0x04
    C3DVector WindVelocity;         // +0x08
    bool WindActive;                // +0x14
    C3DVector CameraPos;            // +0x15
    fable_u8 unknown21_[3];         // tail padding (sizeof 0x24)
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CParticleEnvironmentInteraction) == 0x24);
