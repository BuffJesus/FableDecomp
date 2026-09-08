#include "engine/CAIScratchpadDef.h"

struct CAIScratchpadDef_Methods : CAIScratchpadDef {
    void Copy(const CAIScratchpadDef* src);
    void CopyImpl(const CAIScratchpadDef* src);
};

void CAIScratchpadDef_Methods::Copy(const CAIScratchpadDef* src)
{
    this->CopyImpl(src);
}
