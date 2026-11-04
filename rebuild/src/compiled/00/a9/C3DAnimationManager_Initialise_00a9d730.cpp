// C3DAnimationManager::Initialise / Uninitialise (retail 0x00A9D730 / 0x00A9D740, Ego_r 0x00BE13A0 / 0x00BE13B0): the
// mesh bank creates its animation manager and hands it the bank (caller 0x00A27030); bank shutdown detaches it
// (caller 0x00A27350).
#include "fable_3d_animation_manager.h"

void C3DAnimationManager::Initialise(CMeshDataBank* meshBank)
{
    MeshBank = meshBank;
}

void C3DAnimationManager::Uninitialise()
{
    MeshBank = 0;
}
