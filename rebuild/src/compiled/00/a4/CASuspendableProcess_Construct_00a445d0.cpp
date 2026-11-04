// CASuspendableProcess construction unit (retail 0x00A445D0..0x00A44A3C): constructor, the default
// SuspendableProcess hook and the scalar deleting destructor emitted with the vtable.
#include "fable_suspendable_process.h"

// 0x00A445D0
CASuspendableProcess::CASuspendableProcess(bool terminateUsingExceptions, long, float timesliceLength)
    : Running(false), StopRunning(false), TerminateUsingExceptions(terminateUsingExceptions),
      TimesliceLength(timesliceLength)
{
}

// 0x00A44600
void CASuspendableProcess::SuspendableProcess()
{
}
