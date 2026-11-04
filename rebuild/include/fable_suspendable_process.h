#pragma once
// Hand-owned (retail model): CASuspendableProcess, the base of long-running work (level loading, file reads) that
// can yield and be terminated. Ego_r.pdb 0x10 : CBaseClassNonCopyable; protected Running +0x4, StopRunning +0x5,
// TerminateUsingExceptions +0x6, TimeProcessSliceStarted +0x8, TimesliceLength +0xC; static PActiveProcess and
// NoActiveProcesses. Retail vtable 0x0129D428 (RTTI .?AVCASuspendableProcess@@): ??_G 0x00A44A20, three pure
// slots, SuspendableProcess 0x00A44600. Constructor 0x00A445D0, TerminateProcess 0x00A4B200. The destructor is
// implicit (??_G calls ~CBaseClassNonCopyable with no own vptr store).
#include <stddef.h>
#include "fable_base_class.h"

class CASuspendableProcess : public CBaseClassNonCopyable
{
public:
    // Ego_r ??0CASuspendableProcess@@QAE@_NJM@Z; retail ignores the long.
    CASuspendableProcess(bool terminateUsingExceptions, long, float timesliceLength);

    void TerminateProcess();

    virtual void RunSuspendableProcess() = 0;
    virtual bool SuspendSuspendableProcess() = 0;
    virtual void Restart() = 0;
    virtual void SuspendableProcess();

protected:
    friend struct CASuspendableProcessLayoutCheck;
    bool Running;                     // +0x4
    bool StopRunning;                 // +0x5
    bool TerminateUsingExceptions;    // +0x6
    float TimeProcessSliceStarted;    // +0x8
    float TimesliceLength;            // +0xC

    static CASuspendableProcess* PActiveProcess;
    static long NoActiveProcesses;
};

struct CASuspendableProcessLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CASuspendableProcess, Running) == 0x4);
    FABLE_STATIC_ASSERT(offsetof(CASuspendableProcess, StopRunning) == 0x5);
    FABLE_STATIC_ASSERT(offsetof(CASuspendableProcess, TerminateUsingExceptions) == 0x6);
    FABLE_STATIC_ASSERT(offsetof(CASuspendableProcess, TimeProcessSliceStarted) == 0x8);
    FABLE_STATIC_ASSERT(offsetof(CASuspendableProcess, TimesliceLength) == 0xC);
};
FABLE_STATIC_ASSERT(sizeof(CASuspendableProcess) == 0x10);
