#include <stdio.h>
#include <string.h>
#include "candidates/landscape_patch_relocation_compiled.h"

int main()
{
    CEngineLandscapePatch patch;
    CEngineLandscapePatchRelocationState state;
    memset(&patch, 0, sizeof(patch));
    memset(&state, 0, sizeof(state));
    patch.RelocationState = &state;
    if (!patch.CanRelocateData(0)) return 1;
    state.RelocationBlocked = 1;
    if (patch.CanRelocateData(0)) return 2;
    printf("LANDSCAPE_PATCH_CAN_RELOCATE_TEST PASS\n");
    return 0;
}
