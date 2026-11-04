// CParticleProgram::WriteToFile (retail 0x00AE2230, Ego_r 0x00BEEE80): the inverse of ReadFromFile - format version
// 0x14D (ReadFromFile skips it), the compiled constraint stream (length + bytes), the initial simulation, the render
// triangles / quads / vertices (count + block each), the non-simulated positions (count + WriteVectorArray), the texture
// coordinates and both vertex maps (count + block), the average patch size, the Bezier flag, the exported particles
// (count, then name and particle per entry) and both group lists.
#include "fable_particle_program.h"
#include "fable_data_output_stream.h"

void CParticleProgram::WriteToFile(CDataOutputStream& stream) const
{
    stream.WriteULONG(0x14D);
    stream.WriteULONG(Constraints.GetLength());
    stream.Write(Constraints.PeekData(), Constraints.GetLength());
    InitialSimulation.WriteToFile(stream);

    stream.WriteULONG(RenderTris.size());
    stream.Write(RenderTris.GetAsCArray(), RenderTris.size() * sizeof(C3DTriangle2));
    stream.WriteULONG(RenderQuads.size());
    stream.Write(RenderQuads.GetAsCArray(), RenderQuads.size() * sizeof(C3DQuad2));
    stream.WriteULONG(RenderVertices.size());
    stream.Write(RenderVertices.GetAsCArray(), RenderVertices.size() * sizeof(C3DClothRenderVertex));

    stream.WriteULONG(NonSimCount);
    CParticleSimulation::WriteVectorArray(NonSimPositions, NonSimCount, stream);

    stream.WriteULONG(IndexedTextureCoords.size());
    stream.Write(IndexedTextureCoords.GetAsCArray(), IndexedTextureCoords.size() * sizeof(C2DVector));
    stream.WriteULONG(ParticleIndices.size());
    stream.Write(ParticleIndices.GetAsCArray(), ParticleIndices.size() * sizeof(long));
    stream.WriteULONG(VertexIndices.size());
    stream.Write(VertexIndices.GetAsCArray(), VertexIndices.size() * sizeof(long));
    stream.WriteFloat(AveragePatchSize);
    stream.WriteEBOOL(BezierEnable);

    stream.WriteULONG(ExportParticles.size());
    for (unsigned long i = 0; i < ExportParticles.size(); ++i)
    {
        stream.WriteNullTerminatedString(ExportParticles[i].first);
        stream.WriteULONG(ExportParticles[i].second);
    }

    WriteGroups(stream, SimGroups);
    WriteGroups(stream, NonSimGroups);
}
