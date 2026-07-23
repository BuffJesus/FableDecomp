#include "candidates/landscape_patch_relocation_compiled.h"

bool CEngineLandscapePatch::CanRelocateData(long) const
{
    return RelocationState->RelocationBlocked == 0;
}
