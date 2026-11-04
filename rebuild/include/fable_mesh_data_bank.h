#pragma once
// Hand-owned mesh data bank family. The generated engine/CMeshDataBankEntry.h,
// engine/C3DMeshInfo.h and engine/C3DAnimationInfo.h and the hand-kept engine/CMeshDataBank.h
// are reduced to include shims of this header, so tools/decomp_pipeline/gen_class_headers.py
// (hand_owned(): classes defined in a root include/*.h) leaves these classes alone.
// Names, members and member order: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) and its
// MeshDataBank.cpp symbols; constructor/method signatures follow the Ego_r/FableWin manglings.
// Virtual slots: retail RTTI vtables (CMeshDataBankEntry 0x0129CD28, C3DMeshInfo 0x0129CD3C,
// CPhysicsMeshInfo 0x0129CD50, C3DAnimationInfo 0x0129CD64, CPhysicsMeshInfo::CMeshData
// 0x0129CD78, C3DMeshLODInfo 0x0129CDA4, C3DMeshInfo::CMeshData 0x0129CDD0,
// C3DAnimationInfo::CAnimationData 0x0129CE50, CMeshBankStateBlock 0x0129CE7C, CMeshDataBank
// 0x0129CE94). Retail offsets differ from Ego_r wherever Ego_r embeds an MS-STL container
// (16 bytes) that retail's STLport builds as 12 bytes.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_game_text_bank.h"
#include "fable_carray.h"
#include "fable_counted_pointer.h"
#include "fable_iv_counted_pointer.h"
#include "fable_resource_list.h"
#include "fable_stlport_tree.h"
#include "fable_font_state_blocks.h"
#include "fable_gfp_sizeof.h"
#include "fable_2d_vector.h"
#include "fable_bounding_volumes.h"
#include "fable_scoped_ptr.h"
#include "fable_bank_load_priority.h"

class CMeshDataBank;
class C3DMesh2;
class C3DAnimation2;
class C3DAnimationManager;
class CPhysicsMesh;
class CGraphicDataBank;
class CBankFileAsyncData;
class C3DMeshFileXLoader;
class C3DAnimFileXLoader;
class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;
class C3DQuadricBasedMeshOptimiser;
class C3DMeshStripifier;
class CPushBufferBuildInfo;

// Ego_r CMeshDataBankEntry (0x0c). Retail vtable 0x0129CD28: [0] ??_G 0x00A23910, which stores
// this vptr itself (the destructor is inline: every derived destructor ends by restoring it,
// e.g. ~C3DMeshInfo 0x00A24991), [1] FreeDeviceSpecificResources 0x00A23900, [2] Sizeof and
// [3] Reset pure (_purecall 0x00BFEA08).
class CMeshDataBankEntry
{
public:
    // Ego_r ??0CMeshDataBankEntry@@QAE@KPAVCMeshDataBank@@@Z.
    CMeshDataBankEntry(unsigned long index, CMeshDataBank* bank);
    virtual ~CMeshDataBankEntry()
    {
    }
    virtual void FreeDeviceSpecificResources();
    virtual unsigned long Sizeof() const = 0;
private:
    virtual void Reset() = 0;
public:
    // FableWin ?GetIndex@CMeshDataBankEntry@@QBEKXZ (inline in retail).
    unsigned long GetIndex() const
    {
        return Index;
    }
    // FableWin ?GetName@CMeshDataBankEntry@@QBE?AVCCharString@@XZ / ?GetDataType@...@@QBEKXZ
    // (bodies not recovered).
    CCharString GetName() const;
    unsigned long GetDataType() const;
protected:
    // FableWin ?GetBank@CMeshDataBankEntry@@IBEPAVCMeshDataBank@@XZ and
    // ?PeekBank@CMeshDataBankEntry@@IBEPBVCMeshDataBank@@XZ (protected; inline in retail, e.g.
    // C3DMeshInfo::PeekPhysicsMesh 0x00A24A70, FableWin 0x0313D78A).
    CMeshDataBank* GetBank() const
    {
        return Bank;
    }
    const CMeshDataBank* PeekBank() const
    {
        return Bank;
    }
private:
    CMeshDataBank* Bank;                                     // +0x04
    unsigned long Index;                                     // +0x08
    friend struct CMeshDataBankEntryLayout;
};
// offsetof needs access to the private members; this friend only pins the layout.
struct CMeshDataBankEntryLayout
{
    enum
    {
        BankOffset = offsetof(CMeshDataBankEntry, Bank),
        IndexOffset = offsetof(CMeshDataBankEntry, Index)
    };
};
FABLE_STATIC_ASSERT(sizeof(CMeshDataBankEntry) == 0x0c);
FABLE_STATIC_ASSERT(CMeshDataBankEntryLayout::BankOffset == 0x04);
FABLE_STATIC_ASSERT(CMeshDataBankEntryLayout::IndexOffset == 0x08);

class C3DMeshInfo;

// Ego_r C3DMeshLODInfo (0x34) : CResource. Retail vtable 0x0129CDA4: [0] ??_G 0x00A2B690,
// [2] UnloadResource 0x00A254F0, [4] IsResourceLoaded 0x00A23DE0, [5] OnResourceLoaded
// 0x00A25470, [6] CanAbortResourceLoad 0x00A23C20, [7] OnResourceLoadAborted 0x00A254A0
// (slots 1/3/8/9 are CIVCountedPointeeBase/CResource's).
class C3DMeshLODInfo : public CResource
{
    friend class C3DMeshInfo;
    // Ego_r ??0C3DMeshLODInfo@@AAE@PBVC3DMeshInfo@@K@Z (private).
    C3DMeshLODInfo(const C3DMeshInfo* meshInfo, unsigned long lod);
public:
    // Ego_r ??1C3DMeshLODInfo@@UAE@XZ, retail 0x00A24470.
    virtual ~C3DMeshLODInfo();
    virtual bool UnloadResource();
    virtual bool IsResourceLoaded() const;
    virtual void OnResourceLoaded();
    virtual bool CanAbortResourceLoad(long priority) const;
    virtual void OnResourceLoadAborted();
    // Ego_r ?Sizeof@C3DMeshLODInfo@@QBEKXZ.
    unsigned long Sizeof() const;
private:
    // Ego_r ?IsLoaded@C3DMeshLODInfo@@ABE_NXZ / ?CanUnload@C3DMeshLODInfo@@ABE_NXZ.
    bool IsLoaded() const;
    bool CanUnload() const;
public:
    CCountedPointer<C3DMesh2> Mesh;                          // +0x1c
    CCountedPointer<CBankFileAsyncData> FileData;            // +0x24
    const C3DMeshInfo* MeshInfo;                             // +0x2c
    unsigned char LOD;                                       // +0x30
    bool UsedMeshStats;                                      // +0x31
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshLODInfo) == 0x34);
FABLE_STATIC_ASSERT(offsetof(C3DMeshLODInfo, Mesh) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(C3DMeshLODInfo, FileData) == 0x24);
FABLE_STATIC_ASSERT(offsetof(C3DMeshLODInfo, MeshInfo) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(C3DMeshLODInfo, LOD) == 0x30);
FABLE_STATIC_ASSERT(offsetof(C3DMeshLODInfo, UsedMeshStats) == 0x31);

class CPhysicsMeshInfo;

// Ego_r C3DMeshInfo (0x60) : CMeshDataBankEntry. Retail vtable 0x0129CD3C: [0] ??_G 0x00A2B6B0,
// [1] FreeDeviceSpecificResources 0x00A23C70, [2] Sizeof 0x00A26170, [3] Reset 0x00A24CD0.
class C3DMeshInfo : public CMeshDataBankEntry
{
public:
    class CMeshData;
    // Ego_r ??0C3DMeshInfo@@QAE@KKPAVCMeshDataBank@@@Z.
    C3DMeshInfo(unsigned long index, unsigned long meshID, CMeshDataBank* bank);
    // Ego_r ??1C3DMeshInfo@@UAE@XZ, retail 0x00A24930.
    virtual ~C3DMeshInfo();
    virtual void FreeDeviceSpecificResources();
    virtual unsigned long Sizeof() const;
private:
    virtual void Reset();
public:
    // FableWin ?GetMaxLODs@C3DMeshInfo@@QBEKXZ (inline in Ego_r and retail: the LOD loops of
    // ~C3DMeshInfo 0x00A24930 and Sizeof 0x00A26170 run to the constant 4).
    unsigned long GetMaxLODs() const
    {
        return 4;
    }
    // Ego_r MeshDataBank.cpp order: HasPhysicsMesh 0x00A23C40, BeginLoadingTextures 0x00A23C50,
    // AreTexturesLoaded 0x00A23C60.
    bool HasPhysicsMesh() const;
    void BeginLoadingTextures(EBankLoadPriority priority) const;
    bool AreTexturesLoaded() const;
    // Ego_r ?PeekPhysicsMesh@C3DMeshInfo@@QBEPBVCPhysicsMeshInfo@@XZ, retail 0x00A24A70.
    const CPhysicsMeshInfo* PeekPhysicsMesh() const;
private:
    // Ego_r ?GetLODInfo@C3DMeshInfo@@ABEPAVC3DMeshLODInfo@@K@Z.
    C3DMeshLODInfo* GetLODInfo(unsigned long lod) const;
public:
    CIVCountedPointer<CMeshData> MeshData;                   // +0x0c
    C3DBoundingBox BoundingBox;                              // +0x10
    C3DBoundingSphere BoundingSphere;                        // +0x28
    float SafeBoundingRadius;                                // +0x38
    long PhysicsMeshIndex;                                   // +0x3c
    float LODErrors[4];                                      // +0x40
    unsigned long LODDataSizes[4];                           // +0x50
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshInfo) == 0x60);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, MeshData) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, BoundingBox) == 0x10);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, BoundingSphere) == 0x28);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, SafeBoundingRadius) == 0x38);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, PhysicsMeshIndex) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, LODErrors) == 0x40);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo, LODDataSizes) == 0x50);

// Ego_r C3DMeshInfo::CMeshData (0x20) : CIVCountedPointeeBase. Retail vtable 0x0129CDD0 (two
// slots: ??_G 0x00A2B6D0, DeleteCIVCountedPointeeBase). Constructor and destructor are the
// implicit ones (FableWin ??0CMeshData@C3DMeshInfo@@QAE@XZ / ??1...): retail keeps them as
// unreferenced COMDATs 0x00A2AAB0 (MeshID left uninitialised) and 0x00A2B6F0 (no vptr store of
// its own; ends by restoring the CIVCountedPointeeBase vptr 0x012354D0).
class C3DMeshInfo::CMeshData : public CIVCountedPointeeBase
{
public:
    CIVCountedPointer<C3DMeshLODInfo> LODMeshes[4];          // +0x08
    CIVCountedPointer<C3DMeshLODInfo> MeshStats;             // +0x18
    unsigned long MeshID;                                    // +0x1c
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshInfo::CMeshData) == 0x20);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo::CMeshData, LODMeshes) == 0x08);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo::CMeshData, MeshStats) == 0x18);
FABLE_STATIC_ASSERT(offsetof(C3DMeshInfo::CMeshData, MeshID) == 0x1c);

// Ego_r CPhysicsMeshInfo (0x10) : CMeshDataBankEntry. Retail vtable 0x0129CD50: [0] ??_G
// 0x00A2B740, [1] the inherited CMeshDataBankEntry::FreeDeviceSpecificResources 0x00A23900,
// [2] Sizeof 0x00A26370, [3] Reset 0x00A24F90.
class CPhysicsMeshInfo : public CMeshDataBankEntry
{
public:
    class CMeshData;
    // Ego_r ??0CPhysicsMeshInfo@@QAE@KKPAVCMeshDataBank@@@Z.
    CPhysicsMeshInfo(unsigned long index, unsigned long meshID, CMeshDataBank* bank);
    // Ego_r ??1CPhysicsMeshInfo@@UAE@XZ.
    virtual ~CPhysicsMeshInfo();
    virtual unsigned long Sizeof() const;
private:
    virtual void Reset();
public:
    // Ego_r ?IsLoaded@CPhysicsMeshInfo@@QBE_NXZ / ?BeginLoading@CPhysicsMeshInfo@@QBEXXZ.
    bool IsLoaded() const;
    void BeginLoading() const;
private:
    // Ego_r ?IsLoading@CPhysicsMeshInfo@@ABE_NXZ.
    bool IsLoading() const;
public:
    CIVCountedPointer<CMeshData> MeshData;                   // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshInfo) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshInfo, MeshData) == 0x0c);

// Ego_r CPhysicsMeshInfo::CMeshData (0x2c) : CResource. Retail vtable 0x0129CD78: [0] ??_G
// 0x00A2B610, [2] UnloadResource 0x00A24FC0, [4] IsResourceLoaded 0x00A23FF0, [5]
// OnResourceLoaded 0x00A25E60. Constructor: FableWin ??0CMeshData@CPhysicsMeshInfo@@QAE@PBV1@@Z
// (inline; retail keeps an unreferenced COMDAT copy at 0x00A2AA80). The destructor is implicit
// (retail COMDAT 0x00A2B630 stores no vptr of its own and tail-calls ~CResource 0x009FC550).
class CPhysicsMeshInfo::CMeshData : public CResource
{
public:
    CMeshData(const CPhysicsMeshInfo* owner);
    virtual bool UnloadResource();
    virtual bool IsResourceLoaded() const;
    virtual void OnResourceLoaded();
    // Ego_r ?Sizeof@CMeshData@CPhysicsMeshInfo@@QBEKXZ / ?BuildMesh@CMeshData@CPhysicsMeshInfo@@QAEXXZ.
    unsigned long Sizeof() const;
    void BuildMesh();

    CCountedPointer<CBankFileAsyncData> FileData;            // +0x1c
    CIVCountedPointer<CPhysicsMesh> Mesh;                    // +0x24
    const CPhysicsMeshInfo* Owner;                           // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CPhysicsMeshInfo::CMeshData) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshInfo::CMeshData, FileData) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshInfo::CMeshData, Mesh) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CPhysicsMeshInfo::CMeshData, Owner) == 0x28);

// Ego_r C3DAnimationInfo (0x28) : CMeshDataBankEntry. Retail vtable 0x0129CD64: [0] ??_G
// 0x00A2BD70, [1] the inherited 0x00A23900, [2] Sizeof 0x00A265B0, [3] Reset 0x00A24D40.
class C3DAnimationInfo : public CMeshDataBankEntry
{
public:
    class CAnimationData;
    // Ego_r ??0C3DAnimationInfo@@QAE@KKPAVCMeshDataBank@@@Z.
    C3DAnimationInfo(unsigned long index, unsigned long animationID, CMeshDataBank* bank);
    // Ego_r ??1C3DAnimationInfo@@UAE@XZ, retail 0x00A25970.
    virtual ~C3DAnimationInfo();
    virtual unsigned long Sizeof() const;
private:
    virtual void Reset();
public:
    // Ego_r ?GetNonLoopingDuration@C3DAnimationInfo@@QBEMXZ, retail 0x00A23CE0.
    float GetNonLoopingDuration() const;
    // Ego_r ?IsLoading@C3DAnimationInfo@@QBE_NXZ, retail 0x00A23F30.
    bool IsLoading() const;

    CIVCountedPointer<CAnimationData> Data;                  // +0x0c
    float Duration;                                          // +0x10
    float NonLoopingDuration;                                // +0x14
    C3DVector MovementVector;                                // +0x18
    float Rotation;                                          // +0x24
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationInfo) == 0x28);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, Data) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, Duration) == 0x10);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, NonLoopingDuration) == 0x14);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, MovementVector) == 0x18);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo, Rotation) == 0x24);

// The Animations tree's lifetime as retail expands it in ~CAnimationData 0x00A26860 (STLport
// ~_Rb_tree: clear, which calls the out-of-line node erase 0x00A2B9D0, then release the header
// node). The shared generic ~map() in fable_stlport_tree.h is empty, so this instantiation's
// destructor is specialised here, in the header that owns the member, so every unit that destroys
// a CAnimationData sees the same definition (precedent: map<CCharString,long>::~map() in
// fable_stlport_tree.h).
namespace _STL {
template<> inline map<unsigned long, CCountedPointer<C3DAnimation2> >::~map()
{
    _M_t.clear();
    if (_M_t._M_header._M_data != 0)
        _M_t._M_header.deallocate(_M_t._M_header._M_data, 1);
}
}

// Ego_r C3DAnimationInfo::CAnimationData (0x40 with its MS-STL map; 0x3c in retail, whose
// STLport map is 0x0c) : CResource. Retail vtable 0x0129CE50: [0] ??_G 0x00A2DB10, [2]
// UnloadResource 0x00A25AE0, [4] IsResourceLoaded 0x00A23FC0, [5] OnResourceLoaded 0x00A259F0.
class C3DAnimationInfo::CAnimationData : public CResource
{
public:
    // Ego_r ??0CAnimationData@C3DAnimationInfo@@QAE@PBV1@@Z.
    CAnimationData(const C3DAnimationInfo* animInfo);
    // Ego_r ??1CAnimationData@C3DAnimationInfo@@UAE@XZ, retail 0x00A26860.
    virtual ~CAnimationData();
    virtual bool UnloadResource();
    virtual bool IsResourceLoaded() const;
    virtual void OnResourceLoaded();
    // ?Sizeof@CAnimationData@C3DAnimationInfo@@QBEKXZ, retail 0x00A25B20.
    unsigned long Sizeof() const;
    // ?UpdateSize@CAnimationData@C3DAnimationInfo@@QAEXXZ, retail 0x00A26290.
    void UpdateSize();
    bool CanUnload() const;
    void BuildSharedAnimation();

    _STL::map<unsigned long, CCountedPointer<C3DAnimation2> > Animations; // +0x1c
    CCountedPointer<C3DAnimation2> SharedAnimation;          // +0x28
    CCountedPointer<CBankFileAsyncData> FileData;            // +0x30
    const C3DAnimationInfo* AnimInfo;                        // +0x38
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationInfo::CAnimationData) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo::CAnimationData, Animations) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo::CAnimationData, SharedAnimation) == 0x28);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo::CAnimationData, FileData) == 0x30);
FABLE_STATIC_ASSERT(offsetof(C3DAnimationInfo::CAnimationData, AnimInfo) == 0x38);

// Ego_r CMeshBankStateBlock (0x3c; 0x34 in retail) : CBankStateBlock. Retail vtable 0x0129CE7C:
// [0] ??_G 0x00A2D770, [1] GetDefault 0x00A23A30, [2] CreateCRC 0x00A25040, [3] Set 0x00A2D6C0.
// The constructor (retail COMDAT 0x00A2D650; Ego_r ??0CMeshBankStateBlock@@QAE@XZ 0x009DCF50, a
// COMDAT outside MeshDataBank.cpp's own run) and the destructor (FableWin
// ??1CMeshBankStateBlock@@UAE@XZ; retail 0x00A2D710 stores no vptr of its own and ends by
// restoring the base vptr 0x0129C82C) are both implicit, so neither is declared here. They are
// emitted by the CMeshDataBank constructor/destructor unit (retail 0x00A27030 / 0x00A27350),
// which is not reconstructed.
class CMeshBankStateBlock : public CBankStateBlock
{
public:
    // Ego_r CMeshBankStateBlock::CStateBlock (0x10).
    class CStateBlock
    {
    public:
        bool EnableLOD;                                      // +0x00
        bool PreserveMaterials;                              // +0x01
        bool GenericAnimation;                               // +0x02
        bool UnpackedPositions;                              // +0x03
        long MaxLODs;                                        // +0x04
        long ResourcePhysicsMeshIndex;                       // +0x08
        long AnimationVersion;                               // +0x0c
    };
    // Ego_r CMeshBankStateBlock::CLODStateBlock (0x18; 0x14 in retail).
    class CLODStateBlock
    {
    public:
        // Ego_r ?SetBoneLOD@CLODStateBlock@CMeshBankStateBlock@@QAEXABVCCharString@@J@Z.
        void SetBoneLOD(const CCharString& boneName, long lod);

        _STL::map<CCharString, long> BoneLODs;               // +0x00
        CCountedPointer<C3DBoundingBox> MinXYZRange;         // +0x0c
    };
    // Ego_r CMeshBankStateBlock::CAnimationStateBlock (0x10; 0x0c in retail).
    class CAnimationStateBlock
    {
    public:
        // Ego_r ?SetAnimateBone@CAnimationStateBlock@CMeshBankStateBlock@@QAEXABVCCharString@@_N@Z.
        void SetAnimateBone(const CCharString& boneName, bool animate);

        _STL::set<CCharString> ExcludedBones;                // +0x00
    };

    virtual void GetDefault(CBankStateBlock& output);
    virtual unsigned long CreateCRC();
protected:
    virtual void Set(const CBankStateBlock* source);
public:
    CStateBlock State;                                       // +0x04
    CLODStateBlock LODState;                                 // +0x14
    CAnimationStateBlock AnimationState;                     // +0x28
};
FABLE_STATIC_ASSERT(sizeof(CMeshBankStateBlock::CStateBlock) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CMeshBankStateBlock::CLODStateBlock) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CMeshBankStateBlock::CAnimationStateBlock) == 0x0c);
FABLE_STATIC_ASSERT(sizeof(CMeshBankStateBlock) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CMeshBankStateBlock, State) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CMeshBankStateBlock, LODState) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CMeshBankStateBlock, AnimationState) == 0x28);

// Ego_r CMeshDataBank (0x4f8) : CBankFileAsync (0x164); retail size 0x460 (CMeshManager
// 0x00A09F6E allocates 0x460 bytes for the constructor 0x00A27030). Members follow Ego_r's order.
// Every offset is retail's: derived with retail's 12-byte STLport containers (Ego_r's MS-STL
// CArray/map are 16 bytes) and checked against retail bodies:
//  - ~CMeshDataBank 0x00A27350 deletes MeshLoader +0x164 / AnimLoader +0x168, destroys Filename
//    +0x16C, runs ~CResourceBank 0x009FC690 on +0x170 / +0x1EC / +0x268 / +0x2E4, releases the
//    pools at +0x360..+0x378, MeshStripifier +0x3F8 and PushBufferBuildInfo +0x41C, frees the
//    CArrays at +0x380..+0x3B0, destroys AnimationEnumType +0x3BC, releases AnimationManager
//    +0x3C0, erases LipSyncAnimations +0x3C8 and destroys StateBlock +0x42C (0x00A2D710);
//  - the constructor builds StateBlock at +0x42C (0x00A271AB calls 0x00A2D650);
//  - PeekStaticMesh 0x00A24280 reads MeshesAndAnimations +0x380, NotifyEntryAdded 0x00A26F48
//    resizes DynamicInfoBuffer +0x3B0, SetTextureBank 0x00A23B50 / SetDP3BumpEnable 0x00A23B60
//    store +0x3D4 / +0x3D8.
// Ego_r offsets of the anchored members: MeshLoader +0x1D8, MeshesAndAnimations +0x3F4,
// DynamicInfoBuffer +0x434, LipSyncAnimations +0x450, TextureBank +0x460, DP3BumpEnable +0x464,
// StateBlock +0x4BC.
// Only the virtual overrides the recovered bodies need are declared (slots 5, 8 and 16 of retail
// vtable 0x0129CE94). Retail also overrides slots 10 (0x00A26660), 12 (0x00A26D40), 13
// (0x00A243B0), 14 (0x00A24330) and 19 (0x00A23D80) and appends 0x00A25420 / 0x00A276D0, so no
// unit may emit this vtable (??_GCMeshDataBank 0x00A2E370, the constructor or ~CMeshDataBank)
// until the full list is declared.
class CMeshDataBank : public CBankFileAsync
{
public:
    // Ego_r CMeshDataBank::CLipSyncEntry (0x08): the element of LipSyncAnimations' CArray
    // values (retail _M_erase 0x00A2DC20 destroys PhonemeName in each).
    class CLipSyncEntry
    {
    public:
        CCharString PhonemeName;                             // +0x00
        unsigned long AnimationIndex;                        // +0x04
    };

    // Ego_r ??0CMeshDataBank@@QAE@XZ / ??1CMeshDataBank@@UAE@XZ (retail 0x00A27350, called
    // by ??_G 0x00A2E370).
    CMeshDataBank();
    virtual ~CMeshDataBank();
    // Retail vtable 0x0129CE94 slot 5 (0x00A26C50).
    virtual void Close();
    // Ego_r MeshDataBank.cpp order (b5fdd0, b5fde0) = retail 0x00A23B50, 0x00A23B60.
    void SetTextureBank(const CGraphicDataBank* textureBank);
    void SetDP3BumpEnable(bool enable);
    // Ego_r b60250 = retail 0x00A24000 (only caller 0x00A09DA3 walks the bank list).
    void FreeDeviceSpecificResources();
    // Ego_r MeshDataBank.cpp order (b60500..b60570) = retail 0x00A24250..0x00A242C0, one
    // 16-byte body each over MeshesAndAnimations.
    const CPhysicsMeshInfo* PeekPhysicsMeshInfo(unsigned long index) const;
    const C3DMeshInfo* PeekMeshInfo(unsigned long index) const;
    const C3DMeshInfo* PeekMesh(unsigned long index) const;
    const C3DMeshInfo* PeekStaticMesh(unsigned long) const;
    const C3DMeshInfo* PeekRepeatedMesh(unsigned long) const;
    const C3DMeshInfo* PeekParticleMesh(unsigned long index) const;
    const C3DMeshInfo* PeekAnimatedMesh(unsigned long index) const;
    const C3DAnimationInfo* PeekAnimation(unsigned long) const;
protected:
    // Retail vtable slot 8 (0x00A23AC0); Ego_r ?GetDataFormatVersion@CMeshDataBank@@MAEKXZ.
    virtual unsigned long GetDataFormatVersion();
    // Retail vtable slot 16 (0x00A24410).
    virtual void PrepareDataForInclusion(unsigned long, const CArray<CCharString>&,
        unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&);
private:
    // ?CloseMeshes@CMeshDataBank@@AAEXXZ, retail 0x00A26900.
    void CloseMeshes();
public:
    boost::scoped_ptr<C3DMeshFileXLoader> MeshLoader;        // +0x164
    boost::scoped_ptr<C3DAnimFileXLoader> AnimLoader;        // +0x168
    CCharString Filename;                                    // +0x16c
    CResourceBank MeshResourceBank;                          // +0x170
    CResourceBank MeshStatsResourceBank;                     // +0x1ec
    CResourceBank AnimationResourceBank;                     // +0x268
    CResourceBank PhysicsMeshResourceBank;                   // +0x2e4
    CCountedPointer<CMovableResourceMemoryPool> AnimationMemoryPool; // +0x360
    CCountedPointer<CMovableResourceMemoryPool> ClothMemoryPool;     // +0x368
    CCountedPointer<CMovableResourceMemoryPool> MeshDataMemoryPool;  // +0x370
    CCountedPointer<CXboxBufferMemoryPool> XboxMemoryPool;   // +0x378
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;         // +0x380
    CArray<C3DMeshInfo> MeshInfoBuffer;                      // +0x38c
    CArray<CPhysicsMeshInfo> PhysicsMeshInfoBuffer;          // +0x398
    CArray<C3DAnimationInfo> AnimationInfoBuffer;            // +0x3a4
    CArray<CCountedPointer<CMeshDataBankEntry> > DynamicInfoBuffer; // +0x3b0
    CCharString AnimationEnumType;                           // +0x3bc
    CCountedPointer<C3DAnimationManager> AnimationManager;   // +0x3c0
    _STL::map<CCharString, CArray<CLipSyncEntry> > LipSyncAnimations; // +0x3c8
    const CGraphicDataBank* TextureBank;                     // +0x3d4
    bool DP3BumpEnable;                                      // +0x3d8
    CArray<unsigned long> GroupBlockSizes;                   // +0x3dc
    unsigned long MaxBonesPerVertex;                         // +0x3e8
    unsigned long MaxParticleMeshFacesToRepeat;              // +0x3ec
    CCountedPointer<C3DQuadricBasedMeshOptimiser> MeshOptimiser; // +0x3f0
    CCountedPointer<C3DMeshStripifier> MeshStripifier;       // +0x3f8
    float MaximumLODErrors[4];                               // +0x400
    float MiminumLODReductionPercent;                        // +0x410 (Ego_r spelling)
    unsigned long MiminumLODReductionFaces;                  // +0x414
    float MinimumAnimationMatch;                             // +0x418
    CCountedPointer<CPushBufferBuildInfo> PushBufferBuildInfo; // +0x41c
    long UnloadableMeshMemory;                               // +0x424
    long UnloadMeshFrameDelay;                               // +0x428
    CMeshBankStateBlock StateBlock;                          // +0x42c
};
FABLE_STATIC_ASSERT(sizeof(CResourceBank) == 0x7c);
FABLE_STATIC_ASSERT(sizeof(CMeshDataBank) == 0x460);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, MeshLoader) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, AnimLoader) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, Filename) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, MeshResourceBank) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, MeshStatsResourceBank) == 0x1ec);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, AnimationResourceBank) == 0x268);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, PhysicsMeshResourceBank) == 0x2e4);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, AnimationMemoryPool) == 0x360);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, XboxMemoryPool) == 0x378);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, AnimationEnumType) == 0x3bc);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, AnimationManager) == 0x3c0);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, MeshStripifier) == 0x3f8);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, PushBufferBuildInfo) == 0x41c);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, StateBlock) == 0x42c);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, MeshesAndAnimations) == 0x380);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, DynamicInfoBuffer) == 0x3b0);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, LipSyncAnimations) == 0x3c8);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, TextureBank) == 0x3d4);
FABLE_STATIC_ASSERT(offsetof(CMeshDataBank, DP3BumpEnable) == 0x3d8);
