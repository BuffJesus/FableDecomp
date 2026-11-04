// CParticleProgram::ReadFromFile (retail 0x00AE3C30, Ego_r 0x00BF2EE0): load a cloth program - a (skipped) version word,
// the compiled constraint stream, the initial simulation, the render triangles/quads/vertices, then (once the pool has room
// for every particle) the non-simulated positions, the texture coordinates, both vertex maps, the patch size and Bezier
// flag, the named export particles and finally both group lists. Fails, leaving the rest unread, when either the
// simulation or the particle memory cannot be allocated. As in retail, the export list is resized to the stored count
// before the entries are appended.
#include "fable_particle_program.h"
#include "fable_data_input_stream.h"

#include <new>
#include "fable_stlport_vector_impl.h"   // retail expands the export list's push_back here

// Retail calls these vector instantiations out of line (they belong to other units) while the push_back above
// expands: the one-argument resizes of vector<C3DQuad2> 0x00AE7AA0, vector<C2DVector> 0x00AE7B10 and the export list
// 0x00AE7D10; the two-argument resizes of vector<C3DTriangle2> 0x00A97A50, vector<C3DClothRenderVertex> 0x00AE7830
// and vector<long> 0x004428F0; and the export list's reallocating insert 0x009B71B0.
namespace _STL
{
typedef pair<CCharString, unsigned long> CExportParticle;
template <> void vector<C3DQuad2, allocator<C3DQuad2> >::resize(size_t __new_size);
template <> void vector<C2DVector, allocator<C2DVector> >::resize(size_t __new_size);
template <> void vector<CExportParticle, allocator<CExportParticle> >::resize(size_t __new_size);
template <> void vector<C3DTriangle2, allocator<C3DTriangle2> >::resize(size_t __new_size, const C3DTriangle2& __x);
template <> void vector<C3DClothRenderVertex, allocator<C3DClothRenderVertex> >::resize(size_t __new_size, const C3DClothRenderVertex& __x);
template <> void vector<long, allocator<long> >::resize(size_t __new_size, const long& __x);
template <> void vector<CExportParticle, allocator<CExportParticle> >::_M_insert_overflow(CExportParticle* __position, const CExportParticle& __x, const __false_type&, size_t __fill_len, bool __atend);
}

bool CParticleProgram::ReadFromFile(CDataInputStream& stream, CMovableResourceMemoryPool& pool)
{
    stream.ReadULONG();
    Constraints.Clear();
    unsigned long constraintSize = stream.ReadULONG();
    stream.CopyTo(Constraints, constraintSize);
    if (!InitialSimulation.ReadFromFile(stream, pool))
        return false;

    RenderTris.resize(stream.ReadULONG());
    stream.Read(RenderTris.GetAsCArray(), RenderTris.size() * sizeof(C3DTriangle2));
    RenderQuads.resize(stream.ReadULONG());
    stream.Read(RenderQuads.GetAsCArray(), RenderQuads.size() * sizeof(C3DQuad2));
    RenderVertices.resize(stream.ReadULONG());
    stream.Read(RenderVertices.GetAsCArray(), RenderVertices.size() * sizeof(C3DClothRenderVertex));

    NonSimCount = stream.ReadULONG();
    if (!AllocateArrays(NonSimCount + InitialSimulation.Size, pool))
        return false;
    CParticleSimulation::ReadVectorArray(NonSimPositions, NonSimCount, stream, 0.0f);

    IndexedTextureCoords.resize(stream.ReadULONG());
    stream.Read(IndexedTextureCoords.GetAsCArray(), IndexedTextureCoords.size() * sizeof(C2DVector));
    ParticleIndices.resize(stream.ReadULONG());
    stream.Read(ParticleIndices.GetAsCArray(), ParticleIndices.size() * sizeof(long));
    VertexIndices.resize(stream.ReadULONG());
    stream.Read(VertexIndices.GetAsCArray(), VertexIndices.size() * sizeof(long));
    AveragePatchSize = stream.ReadFloat();
    BezierEnable = stream.ReadEBOOL();

    unsigned long exports = stream.ReadULONG();
    ExportParticles.resize(exports);
    for (unsigned long i = 0; i < exports; ++i)
    {
        CCharString name = stream.ReadNullTerminatedString();
        unsigned long particle = stream.ReadULONG();
        ExportParticles.Insert(name, particle);
    }
    ExportParticles.Update();

    ReadGroups(stream, SimGroups);
    ReadGroups(stream, NonSimGroups);
    return true;
}
