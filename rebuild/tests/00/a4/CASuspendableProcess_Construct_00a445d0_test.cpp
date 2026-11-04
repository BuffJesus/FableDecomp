// Behaviour fixture for the CASuspendableProcess construction unit 0x00A445D0..0x00A44A3C: the constructor and
// SuspendableProcess come from the provider object; a derived test process supplies the pure slots.
#include "fable_suspendable_process.h"
#include <stdio.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}

struct CTestProcess : CASuspendableProcess
{
    CTestProcess(bool exceptions, float timeslice) : CASuspendableProcess(exceptions, 99, timeslice) {}
    virtual void RunSuspendableProcess() {}
    virtual bool SuspendSuspendableProcess() { return false; }
    virtual void Restart() {}
    bool IsRunning() const { return Running; }
    bool IsStopping() const { return StopRunning; }
    bool UsesExceptions() const { return TerminateUsingExceptions; }
    float Timeslice() const { return TimesliceLength; }
};

int main()
{
    CTestProcess p(true, 0.25f);
    CHECK(!p.IsRunning() && !p.IsStopping() && p.UsesExceptions() && p.Timeslice() == 0.25f);
    CTestProcess q(false, 2.0f);
    CHECK(!q.UsesExceptions() && q.Timeslice() == 2.0f);
    p.SuspendableProcess();
    CHECK(!p.IsRunning() && !p.IsStopping());
    printf(g_fail ? "FAIL\n" : "CASUSPENDABLEPROCESS_CONSTRUCT_OK\n");
    return g_fail != 0;
}
