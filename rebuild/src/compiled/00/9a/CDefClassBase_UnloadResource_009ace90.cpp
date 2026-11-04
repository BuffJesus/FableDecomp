#include "fable_def_class_base.h"
#include "fable_definition_manager.h"
bool CDefClassBase::UnloadResource()
{
    return PDefManager->UnloadDef(GlobalIndex);
}
