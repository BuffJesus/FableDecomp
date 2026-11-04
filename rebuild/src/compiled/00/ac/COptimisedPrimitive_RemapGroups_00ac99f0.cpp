// QuadricOptimiserInternals::COptimisedPrimitive::RemapGroups (retail 0x00AC99F0, Ego_r
// lib_3d_quadric_based_mesh_optimiser): renumber the bone groups of every vertex through the old -> new map.
#include "fable_quadric_optimiser.h"

using namespace QuadricOptimiserInternals;

void COptimisedPrimitive::RemapGroups(const _STL::map<unsigned long, unsigned long>& remap)
{
    for (unsigned long i = 0; i < Vertices.size(); ++i)
        Vertices[i].RemapGroups(remap);
}
