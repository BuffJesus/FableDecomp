#include "fable_definition_manager.h"
#include <stdlib.h>

CDefinitionManager::~CDefinitionManager()
{
    ClearInstantiatedDefs();
    // The current shared tree model does not clear nodes in its destructor.
    // Retail destroys these entries before releasing the tree header.
    RecompiledDefs.clear();
    free(RecompiledDefs._M_t._M_header._M_data);
}
