// CParticleProgram::Clear (retail 0x00AE3360, Ego_r 0x00BEF300): empty a particle program - its exported particle names,
// render geometry, constraints, groups and vertex maps - and hand its particle memory back to the movable pool. The
// initial simulation, texture coordinates, temporary positions and loop stack are left as they are.
#include "fable_particle_program.h"
#include "fable_stlport_algobase.h"   // STLport __copy expands where small (quads, render vertices)

void CParticleProgram::Clear()
{
    BezierEnable = false;
    AveragePatchSize = 0.0f;
    ExportParticles.clear();
    RenderTris.clear();
    RenderQuads.clear();
    RenderVertices.clear();
    Constraints.Clear();
    SimGroups.clear();
    NonSimGroups.clear();
    ParticleIndices.clear();
    VertexIndices.clear();
    NonSimCount = 0;
    NonSimPositions = 0;
    if (MemoryHandle != -1)
    {
        MemoryPool->FreeResource(MemoryHandle);
        MemoryHandle = -1;
    }
}
