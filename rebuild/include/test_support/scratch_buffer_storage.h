#pragma once
// Boundary fixtures supply only Lock/Initialise recorders. No global constructor
// is under test; keep their scratch receiver as aligned, zeroed test storage.
#include "fable_system_manager.h"
extern "C" double g_FableScratchBufferTestStorage[(sizeof(CScratchBuffer)+7)/8]={};
#pragma comment(linker,"/alternatename:?GScratchBuffer@@3VCScratchBuffer@@A=_g_FableScratchBufferTestStorage")
