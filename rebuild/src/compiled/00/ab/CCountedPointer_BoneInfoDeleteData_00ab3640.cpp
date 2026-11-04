// CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData (retail 0x00AB3640, Ego_r 0x00C46510): the shared block's
// delete function for the animation builder's bone-hierarchy nodes - destroy the node and free it.
#include "fable_counted_pointer.h"
#include "fable_3d_animation_builder.h"

template void CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData(void*);
