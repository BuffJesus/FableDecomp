// Behaviour fixture for CASuspendableProcess::TerminateProcess 0x00A4B200 (provider object): it requests a stop,
// then runs slices until the process reports it is no longer running. The provider is only TerminateProcess, so
// the base constructor and SuspendableProcess are defined here.
#include "fable_suspendable_process.h"
#include <stdio.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CASuspendableProcess::CASuspendableProcess(bool e, long, float t)
    : Running(false), StopRunning(false), TerminateUsingExceptions(e), TimesliceLength(t) {}
void CASuspendableProcess::SuspendableProcess() {}

struct CTestProcess : CASuspendableProcess
{
    int Slices, Remaining;
    CTestProcess() : CASuspendableProcess(false, 0, 1.0f), Slices(0), Remaining(0) {}
    virtual void RunSuspendableProcess() { ++Slices; if (--Remaining <= 0) Running = false; }
    virtual bool SuspendSuspendableProcess() { return false; }
    virtual void Restart() {}
    bool IsRunning() const { return Running; }
    bool IsStopping() const { return StopRunning; }
    void Start(int slices) { Running = true; Remaining = slices; }
};

int main()
{
    CTestProcess p;
    p.Start(3);
    p.TerminateProcess();
    CHECK(p.IsStopping() && !p.IsRunning() && p.Slices == 3);
    CTestProcess idle;
    idle.TerminateProcess();
    CHECK(idle.IsStopping() && idle.Slices == 0);
    printf(g_fail ? "FAIL\n" : "PARITY_OK\n");
    return g_fail != 0;
}
