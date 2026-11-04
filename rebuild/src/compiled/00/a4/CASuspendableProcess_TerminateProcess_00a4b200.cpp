// CASuspendableProcess::TerminateProcess @ 0x00A4B200: ask the process to stop, then keep running it until it
// reports that it has finished.
#include "fable_suspendable_process.h"

void CASuspendableProcess::TerminateProcess()
{
    StopRunning = true;
    while (Running)
        RunSuspendableProcess();
}
