#pragma once
// Hand-owned (retail model): CParticleProgram, the cloth/particle simulation program a C3DClothPrimitive runs.
// Retail RTTI .?AVCParticleProgram@@ : CMovableResource, vtable 0x0129F0D8.
// Partial view: methods as reconstructed or called (bone queries, group setup, file read), and only the members a
// reconstructed body reads, at retail offsets (Ego_r.pdb's 0x170 layout uses 16-byte CArrays; retail's STLport CArray
// is 12 bytes, so Ego_r offsets shift down). Unmodelled bytes stay opaque inside the retail size 0x140.
#include <stddef.h>
#include "fable_carray.h"
#include "fable_stlport_tree.h"
#include "fable_movable_resource.h"
#include "rebuild_abi.h"
#include "fable_3d_group2.h"
#include "fable_2d_vector.h"
#include "fable_4d_vector.h"
#include "fable_string.h"
#include "fable_vector_map.h"
#include "fable_data_output_stream.h"
#include "fable_particle_simulation.h"

// Ego_r C3DQuad2 (four 16-bit vertex indices) and C3DClothRenderVertex (particle position + texture coordinate
// indices). Both are non-POD to STLport (retail CParticleProgram::Clear erases them element by element).
class C3DQuad2
{
public:
    C3DQuad2() {}
    unsigned short VertexIndex[4];
};
FABLE_STATIC_ASSERT(sizeof(C3DQuad2) == 8);
class C3DClothRenderVertex
{
public:
    C3DClothRenderVertex() {}
    unsigned short PositionIndex;
    unsigned short TexCoordIndex;
};
FABLE_STATIC_ASSERT(sizeof(C3DClothRenderVertex) == 4);

class CDataInputStream;
class CDataOutputStream;
class CMovableResourceMemoryPool;
class CMatrix4x4;
class CParticleContext;
class C3DAnimationBlendState;
class C3DAnimationManager;
class C3DVector;
class C3DMesh2;

class CParticleProgram : public CMovableResource
{
public:
    // Ego_r names; retail 0x00AE3550 (ctor), 0x00AE3C30 (ReadFromFile), 0x00AE3330, 0x00AE37D0.
    CParticleProgram();
    // Ego_r ??1CParticleProgram@@UAE@XZ (retail 0x00AE3670): Clear, then the members unwind. Defined after
    // ~CParticleSimulation in CParticleSimulation_Destructor_00ae0630.cpp (retail expands that destructor inline).
    virtual ~CParticleProgram();
    bool ReadFromFile(CDataInputStream& stream, CMovableResourceMemoryPool& pool);
    // Ego_r ?WriteToFile@CParticleProgram@@QBEXAAVCDataOutputStream@@@Z (retail 0x00AE2230): the inverse of ReadFromFile
    // (format version 0x14D first).
    void WriteToFile(CDataOutputStream& stream) const;
    void ReserveGroups(unsigned long count);
    void AddGroup(const C3DGroup2& group);
    // CMovableResource slot 1 (Ego_r ?RelocateData@CParticleProgram@@UAEXJPAX0J@Z; not yet reconstructed).
    virtual void RelocateData(long handle, void* oldAddress, void* newAddress, long size);
    void GetUsedBones(CArray<bool>& usedBones) const;
    void RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap);
    // Ego_r ?Clear@CParticleProgram@@QAEXXZ (retail 0x00AE3360): empty the program and free its particle memory.
    void Clear();
    // Ego_r ?SaveStateToInterpolateFrom@CParticleProgram@@QAEXAAVCParticleSimulation@@@Z (retail 0x00ADF5B0): copy the
    // simulation's current positions into its saved positions (the program itself is not read).
    void SaveStateToInterpolateFrom(CParticleSimulation& simulation);
    // Ego_r ?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@AAVCMovableResourceMemoryPool@@@Z (retail
    // 0x00AE0050): start a simulation as a copy of the program's initial one.
    bool SetupSimulation(CParticleSimulation& simulation, CMovableResourceMemoryPool& pool) const;
    // Ego_r SimulationStep overloads (retail 0x00AE2CB0 free-standing, 0x00AE2D20 clinging to an animated mesh): drag,
    // forces, pull pinned particles back towards the initial positions (or cling to the mesh), then solve constraints.
    void SimulationStep(CParticleSimulation& simulation, const CParticleContext& context) const;
    void SimulationStep(CParticleSimulation& simulation, const CParticleContext& context,
                        const C3DAnimationBlendState& blendState, C3DAnimationManager& animationManager,
                        const C3DVector* vertices, const C3DMesh2& mesh) const;
    // Ego_r ?Update@CParticleProgram@@QAEXXZ (retail 0x00AE34D0): sort the export list, then recompute the average
    // patch size from the initial simulation.
    void Update();
    // Ego_r ?ClearGroups@CParticleProgram@@QAEXXZ (retail 0x00AE3510): empty both group lists.
    void ClearGroups();
    // Ego_r ?SetData@CParticleProgram@@QAE_NABV?$CArray@VC3DVector@@@@AAVCMovableResourceMemoryPool@@@Z (retail
    // 0x00AE1150): take the non-simulated vertices (w = 0) into pool memory; false when the allocation fails.
    bool SetData(const CArray<C3DVector>& vertices, CMovableResourceMemoryPool& pool);
    // Ego_r PrepareForRendering overloads (retail 0x00AE1600 static mesh, 0x00AE1260 skinned): write the render positions -
    // the non-simulated vertices first, then the simulated particles interpolated between the last two steps by alpha,
    // optionally blended back towards the (skinned) initial positions.
    void PrepareForRendering(C4DVector* positions, CParticleSimulation& simulation, float alpha, float blend);
    void PrepareForRendering(C4DVector* positions, CParticleSimulation& simulation, float alpha, float blend,
                             const C3DAnimationBlendState& blendState, const C3DAnimationBlendState& previousBlendState,
                             float blendFraction, C3DAnimationManager& animationManager, const C3DVector* vertices,
                             const CMatrix4x4* matrices, const C3DMesh2& mesh);

    // Ego_r TConstraintType: the record types of the compiled constraint program (Constraints).
    enum TConstraintType
    {
        CONSTRAINT_REPEAT = 0, CONSTRAINT_END_REPEAT = 1, CONSTRAINT_DISTANCE = 2, CONSTRAINT_MIN_MAX_DISTANCE = 3,
        CONSTRAINT_UNBEND = 4, CONSTRAINT_SPHERE_COLLISION = 5, CONSTRAINT_CYLINDER_COLLISION = 6
    };

    // Ego_r CParticleProgram::CLoopBegin (8 bytes): a loop opened in the particle code being compiled.
    struct CLoopBegin
    {
        unsigned long Position;
        long LoopCount;
    };

private:
    // Ego_r private static helpers (?GetUsedBones@CParticleProgram@@CIX... retail 0x00AE1210,
    // ?RemapBoneIndices@CParticleProgram@@CIX... retail 0x00AE3920) applied to each group list; not yet reconstructed.
    static void FABLE_FASTCALL GetUsedBones(CArray<bool>& usedBones, const CArray<C3DGroup2>& groups);
    static void FABLE_FASTCALL RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap, CArray<C3DGroup2>& groups);
    // Ego_r ?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z (retail 0x00AE3A60).
    static void FABLE_FASTCALL ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups);
    // Ego_r ?WriteGroups@CParticleProgram@@CIXAAVCDataOutputStream@@ABV?$CArray@VC3DGroup2@@@@@Z (retail 0x00AE0680).
    static void FABLE_FASTCALL WriteGroups(CDataOutputStream& stream, const CArray<C3DGroup2>& groups);
    // Ego_r ?AllocateArrays@CParticleProgram@@AAE_NKAAVCMovableResourceMemoryPool@@@Z (retail 0x00ADF460, not yet
    // reconstructed): allocate positions for every particle (simulated + non-simulated) from the pool.
    bool AllocateArrays(unsigned long count, CMovableResourceMemoryPool& pool);
    // Ego_r private helpers, not yet reconstructed: CalculateAveragePatchSize (retail 0x00AE0710), ApplyConstraints
    // (retail 0x00AE2510), ClingToMesh (retail 0x00AE1320).
    void CalculateAveragePatchSize(const CParticleSimulation& simulation);
    // Ego_r ?ExecuteProgram@CParticleProgram@@ABEXAAVCParticleSimulation@@@Z (retail 0x00AE1A90): run the constraint
    // program once over the simulation.
    void ExecuteProgram(CParticleSimulation& simulation) const;
    // Ego_r ?OptimiseConstraints@CParticleProgram@@AAEXXZ (retail 0x00ADFDC0): merge runs of same-type constraint records.
    void OptimiseConstraints();
    void ApplyConstraints(CParticleSimulation& simulation, long maximumIterations, float maximumDistanceError) const;
    // Ego_r ?TransformVertices@CParticleProgram@@ABEXPAVC4DVector@@PBV2@KPBVCMatrix4x4@@ABV?$CArray@VC3DGroup2@@@@@Z
    // (retail 0x00AE0E60, not yet reconstructed).
    void TransformVertices(C4DVector* out, const C4DVector* in, unsigned long count, const CMatrix4x4* matrices,
                           const CArray<C3DGroup2>& groups) const;
    void ClingToMesh(CParticleSimulation& simulation, const C3DAnimationBlendState& blendState,
                     C3DAnimationManager& animationManager, const C3DVector* vertices, const C3DMesh2& mesh) const;

    // Ego_r private static scratch buffers (retail 0x00AE04E0 / 0x00AE0540 / 0x00AE05A0): grow-only, each carved from
    // its own file-static CSimpleMemoryPool (MatrixBufferPool / VectorBufferPool / GroupVectorBufferPool, retail
    // 0x013D2DB0 / 0x013D2DCC / 0x013D2DE8). Statics: retail 0x013D2D98 .. 0x013D2DAC (buffer, size pairs).
    static CMatrix4x4* FABLE_FASTCALL GetMatrixBuffer(unsigned long count);
    static C4DVector* FABLE_FASTCALL GetVectorBuffer(unsigned long count);
    static C4DVector* FABLE_FASTCALL GetGroupVectorBuffer(unsigned long count);
    static CMatrix4x4* MatrixBuffer;
    static unsigned long MatrixBufferSize;
    static C4DVector* VectorBuffer;
    static unsigned long VectorBufferSize;
    static C4DVector* GroupVectorBuffer;
    static unsigned long GroupVectorBufferSize;

    // Retail size 0x140 (C3DClothPrimitive's constructor 0x00AC3E00 news 0x140 bytes; the constructor 0x00AE3550 writes
    // every member). Ego_r member names and order; Ego_r offsets shift down with each 12-byte (retail) CArray. +0x04 is
    // alignment: InitialSimulation holds a double.
    friend struct CParticleProgramLayoutCheck;
    CParticleSimulation InitialSimulation;                                                  // +0x08
    CVectorMap<CCharString, unsigned long, CKeyPairCompareLess<CCharString, unsigned long> > ExportParticles; // +0x88
    CArray<C3DTriangle2> RenderTris;                                                        // +0x98
    CArray<C3DQuad2> RenderQuads;                                                           // +0xA4
    CArray<C3DClothRenderVertex> RenderVertices;                                            // +0xB0
    unsigned long NonSimCount;                                                              // +0xBC
    C4DVector* NonSimPositions;                                                             // +0xC0
    CArray<C2DVector> IndexedTextureCoords;                                                 // +0xC4
    CArray<C4DVector> TempVertexPositions;                                                  // +0xD0
    CArray<long> ParticleIndices;     // +0xDC  source vertex -> particle (-1: not simulated); Ego_r +0xF4
    CArray<long> VertexIndices;       // +0xE8  source vertex -> render vertex (-1: none); Ego_r +0x104
    bool BezierEnable;                // +0xF4
    float AveragePatchSize;           // +0xF8
    CArray<C3DGroup2> SimGroups;      // +0xFC
    CArray<C3DGroup2> NonSimGroups;   // +0x108
    CMemoryDataOutputStream Constraints;   // +0x114
    mutable CArray<CLoopBegin> LoopStack;  // +0x12C (ExecuteProgram, a const member, rebuilds it)
    long MemoryHandle;                     // +0x138 (-1: no allocation)
    CMovableResourceMemoryPool* MemoryPool; // +0x13C
};
struct CParticleProgramLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, InitialSimulation) == 0x08);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, ExportParticles) == 0x88);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, RenderTris) == 0x98);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, NonSimCount) == 0xBC);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, IndexedTextureCoords) == 0xC4);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, ParticleIndices) == 0xDC);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, BezierEnable) == 0xF4);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, Constraints) == 0x114);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, LoopStack) == 0x12C);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, MemoryHandle) == 0x138);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, VertexIndices) == 0xE8);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, SimGroups) == 0xFC);
    FABLE_STATIC_ASSERT(offsetof(CParticleProgram, NonSimGroups) == 0x108);
};
FABLE_STATIC_ASSERT(sizeof(CParticleProgram) == 0x140);
