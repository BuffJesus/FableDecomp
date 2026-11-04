#include "engine/CACompressor.h"
CACompressor::CACompressor(long workspaceSize)
{
    OwnWorkspace.resize(workspaceSize);
    WorkspaceSize = workspaceSize;
    Workspace = OwnWorkspace.begin();
}