#include "candidates/background_tree_node_compiled.h"

void CLandscapeBackgroundTreeNode::UpdateTesselation()
{
    if (!TesselationValid)
    {
        if (AsyncLoadTask != 0)
            AsyncLoadTask->Abort();
        TesselationValid = true;
    }
}
