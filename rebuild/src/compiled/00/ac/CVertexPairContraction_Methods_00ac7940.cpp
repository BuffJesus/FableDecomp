// QuadricOptimiserInternals::CVertexPairContraction (Ego_r lib_3d_quadric_based_mesh_optimiser): RemapIndex
// 0x00AC7940, HasVertex 0x00AC7990 and CleanUp 0x00AC7B00. A contraction is a list of vertex pairs; RemapIndex renames
// one vertex in every pair, HasVertex asks whether any pair uses a vertex, and CleanUp drops repeated pairs, keeping
// the first of each.
#define FABLE_UNIT_INLINE __forceinline   // retail expands the erase copy loop here
#include <new>
#include "fable_stlport_algobase.h"
#include "fable_quadric_optimiser.h"
#include "fable_stlport_vector_impl.h"

using namespace QuadricOptimiserInternals;

void CVertexPairContraction::RemapIndex(unsigned long oldIndex, unsigned long newIndex)
{
    for (unsigned long i = 0; i < SubContractions.size(); ++i)
    {
        CSubContraction& pair = SubContractions[i];
        if (pair.Index1 == oldIndex)
            pair.Index1 = newIndex;
        if (pair.Index2 == oldIndex)
            pair.Index2 = newIndex;
    }
}

bool CVertexPairContraction::HasVertex(unsigned long index)
{
    for (unsigned long i = 0; i < SubContractions.size(); ++i)
    {
        if (SubContractions[i].Index1 == index || SubContractions[i].Index2 == index)
            return true;
    }
    return false;
}

void CVertexPairContraction::CleanUp()
{
    for (unsigned long i = 0; i < SubContractions.size(); ++i)
    {
        const CSubContraction& pair = SubContractions[i];
        for (unsigned long j = i + 1; j < SubContractions.size(); ++j)
        {
            if (pair == SubContractions[j])
            {
                SubContractions.erase(SubContractions.begin() + j);
                --j;
            }
        }
    }
}
