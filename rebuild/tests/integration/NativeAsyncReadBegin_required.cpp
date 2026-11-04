#include <stdlib.h>
#include "fable_game_text_bank.h"
// Qualified genuine public virtual callback caller, link-only and never run.
// SetupBuffer is unrecovered; this abort alias closes only an unused link edge.
extern "C" void UnusedBeginSetupBufferBoundary() { abort(); }
int main(int argc, char** argv)
{
    return reinterpret_cast<CBankFileAsyncData*>(argv)->CBankFileAsyncData::OnFileReadBegin(
        reinterpret_cast<void**>(argv), static_cast<EThreadedFilePriority>(argc));
}
#pragma comment(linker,"/alternatename:?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z=_UnusedBeginSetupBufferBoundary")
