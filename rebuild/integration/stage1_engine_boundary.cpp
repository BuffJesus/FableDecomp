#include "fable_boot.h"

fable_u8* g_FableMicroThreadStackTop_013B7C84 = 0;
fable_u8* g_FableMicroThreadStackBottom_013B7C88 = 0;

namespace MicroThread
{
    void FABLE_FASTCALL SetStack(fable_u8* /* stackTop */, fable_u32 /* stackSize */)
    {
    }
}

extern "C" long FABLE_FASTCALL GFMain(
    FableInstanceHandle /* instance */,
    char* /* commandLine */,
    int /* showCommand */)
{
    // Stage 1 ends here. The real GFMain dependency closure is the next gate.
    return 0;
}
