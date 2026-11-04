#pragma once
// Hand-owned (retail model): CParticleSimulation, the Verlet particle state a CParticleProgram runs (lib_particle).
// Retail RTTI .?AVCParticleSimulation@@ : CMovableResource, vtable 0x0129F0C4. Ego_r.pdb names and members; the layout
// (0x80) has no CArrays, so the Ego_r offsets hold in retail (CParticleSimulation::Clear 0x00ADF1E0 writes them).
// Methods are declared as reconstructed.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_movable_resource.h"
#include "fable_counted_pointer.h"
#include "fable_bounding_volumes.h"
#include "fable_handle_memory_pool.h"
#include "fable_carray.h"

class C4DVector;
class CDataInputStream;
class CDataOutputStream;
class CParticleSimulationRenderData;
class CParticleContext;
class CParticleDistanceConstraint;
class CParticleMinMaxDistanceConstraint;
class CParticleUnbendConstraint;
class CParticleSphereCollisionConstraint;

class CParticleSimulation : public CMovableResource
{
public:
    // Ego_r ??0CParticleSimulation@@QAE@XZ (retail 0x00AE0600): no render data, no allocation, then Clear. Retail's
    // CParticleProgram::CParticleProgram (0x00AE3550) expands it inline (same retail unit), so it is defined just before
    // that constructor in CParticleProgram_CParticleProgram_00ae3550.cpp.
    CParticleSimulation();
    // Ego_r ?Clear@CParticleSimulation@@QAEXXZ (retail 0x00ADF1E0, Ego_r 0x00BEAE90): back to an empty, unallocated
    // simulation at the default 15 Hz timestep.
    void Clear();
    // Ego_r ??1CParticleSimulation@@UAE@XZ (retail 0x00AE0630): Clear, then release the render data. Retail's
    // ~CParticleProgram (0x00AE3670) expands this body inline (one retail TU holds both classes, and /O2 auto-inlines
    // a member defined earlier in the unit), so reconstruct ~CParticleProgram in CParticleSimulation_Destructor_00ae0630.cpp
    // after this destructor.
    virtual ~CParticleSimulation();
    // Ego_r ?CopyFrom@CParticleSimulation@@QAE_NABV1@AAVCMovableResourceMemoryPool@@@Z (retail 0x00ADF610): become a copy
    // of source, particle arrays allocated from pool (false, with only the header copied, when that fails).
    bool CopyFrom(const CParticleSimulation& source, CMovableResourceMemoryPool& pool);
    // Ego_r ?GetBoundingBox@CParticleSimulation@@QBE?AVC3DBoundingBox@@XZ (retail 0x00ADF730): the box, marking it valid.
    C3DBoundingBox GetBoundingBox() const;
    // Ego_r ?SetParticles@CParticleSimulation@@QAEXABV?$CArray@VC3DVector@@@@ABV?$CArray@M@@AAVCMovableResourceMemoryPool@@@Z
    // (retail 0x00AE1030): one particle per position (w = 0, at rest), with its simulation alpha.
    void SetParticles(const CArray<C3DVector>& positions, const CArray<float>& alphas, CMovableResourceMemoryPool& pool);
    // CMovableResource slot 1 (Ego_r ?RelocateData@CParticleSimulation@@UAEXJPAX0J@Z; not yet reconstructed).
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, long size);
    // Ego_r ?ReadFromFile@CParticleSimulation@@QAE_NAAVCDataInputStream@@AAVCMovableResourceMemoryPool@@@Z (retail 0x00AE1BC0).
    bool ReadFromFile(CDataInputStream& stream, CMovableResourceMemoryPool& pool);
    // Ego_r ?WriteToFile@CParticleSimulation@@QBEXAAVCDataOutputStream@@@Z (retail 0x00AE2050): the inverse of ReadFromFile.
    void WriteToFile(CDataOutputStream& stream) const;
    // Ego_r inline accessors for the largest correction of the last constraint pass.
    void ResetMaximumAdjustment() { MaximumAdjustment = 0.0f; }
    float GetMaximumAdjustment() const { return MaximumAdjustment; }

    // CParticleProgram::ReadFromFile (retail 0x00AE3C30) reads its non-simulated positions with ReadVectorArray.
    friend class CParticleProgram;

protected:
    // Ego_r ?ReadVectorArray@CParticleSimulation@@KIXPAVC4DVector@@KAAVCDataInputStream@@M@Z (not yet reconstructed).
    static void FABLE_FASTCALL ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& stream, float w);
    // Ego_r ?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z (retail 0x00AE18C0, not yet
    // reconstructed).
    static void FABLE_FASTCALL WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream);

private:
    // Ego_r ?AllocateArrays@CParticleSimulation@@AAE_NAAVCMovableResourceMemoryPool@@@Z (not yet reconstructed).
    bool AllocateArrays(CMovableResourceMemoryPool& pool);
    // Ego_r ?ApplyDragging@CParticleSimulation@@AAEXABVCParticleContext@@@Z (retail 0x00ADF300) and
    // ?ApplyForces@CParticleSimulation@@AAEXABVCParticleContext@@@Z (retail 0x00ADF760); not yet reconstructed.
    void ApplyDragging(const CParticleContext& context);
    void ApplyForces(const CParticleContext& context);
    // Ego_r constraint solvers (retail SatisfyDistanceConstraints 0x00ADF960, not yet reconstructed; the other three are
    // empty in this build - Ego_r 3-byte stubs - and retail folds their out-of-line copies into 0x00ADF400).
    void SatisfyDistanceConstraints(const CParticleDistanceConstraint* constraints, unsigned long count);
    void SatisfyMinMaxDistanceConstraints(const CParticleMinMaxDistanceConstraint* constraints, unsigned long count);
    void SatisfyUnbendConstraints(const CParticleUnbendConstraint* constraints, unsigned long count);
    void SatisfySphereCollisionConstraints(const CParticleSphereCollisionConstraint* constraints, unsigned long count);

public:

    CCountedPointer<CParticleSimulationRenderData> RenderData;   // +0x04
    float Timestep;                                             // +0x0C
    bool TimestepChanged;                                       // +0x10
    float TimestepMultiplier;                                   // +0x14
    unsigned long Size;                                         // +0x18
    C4DVector* Positions;                                       // +0x1C
    C4DVector* LastPositions;                                   // +0x20
    C4DVector* SavedPositions;                                  // +0x24
    C4DVector* Accelerations;                                   // +0x28
    float* SimulationAlphas;                                    // +0x2C
    float MaximumAdjustment;                                    // +0x30
    float GravityStrength;                                      // +0x34
    float WindStrength;                                         // +0x38
    bool DraggingEnable;                                        // +0x3C
    bool DraggingRotational;                                    // +0x3D
    float DraggingStrength;                                     // +0x40
    bool AccelerationEnable;                                    // +0x44
    float GlobalDamping;                                        // +0x48
    double CurrentTime;                                         // +0x50
    C3DBoundingBox BoundingBox;                                 // +0x58
    mutable bool BoundingBoxValid;                              // +0x70 (GetBoundingBox sets it)
    long MemoryHandle;                                          // +0x74 (-1: no allocation)
    CMovableResourceMemoryPool* MemoryPool;                     // +0x78
};

FABLE_STATIC_ASSERT(sizeof(CParticleSimulation) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CParticleSimulation, Timestep) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CParticleSimulation, GlobalDamping) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CParticleSimulation, CurrentTime) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CParticleSimulation, MemoryHandle) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CParticleSimulation, MemoryPool) == 0x78);
