#include "engine/CAIStateGroup_MinionReactToTrespassing.h"

struct CAIStateGroup_MinionReactToTrespassing_Methods
    : CAIStateGroup_MinionReactToTrespassing {
    bool IsStillRunnable(bool initialCheck);
    bool IsRunnable();
};

bool CAIStateGroup_MinionReactToTrespassing_Methods::IsRunnable()
{
    return this->IsStillRunnable(true);
}
