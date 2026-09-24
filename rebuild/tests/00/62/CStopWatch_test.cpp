#include "fable_frontend_component_types.h"
#include "fable_performance_counter.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static __int64 tick;
static __int64 frequency = 100;
static bool counterWorks = true, frequencyWorks = true;
static unsigned counterCalls;
static int __stdcall Counter(FablePerformanceCounterValue* value)
{
    ++counterCalls;
    if (counterWorks) value->QuadPart = tick;
    return counterWorks;
}
static int __stdcall Frequency(FablePerformanceCounterValue* value)
{
    if (frequencyWorks) value->QuadPart = frequency;
    return frequencyWorks;
}
// Substitute only the OS clock import slots. All seven CStopWatch methods
// linked into this console fixture are the production C++ implementations.
extern "C" {
    int (__stdcall* FableTestCounterImport)(FablePerformanceCounterValue*) = Counter;
    int (__stdcall* FableTestFrequencyImport)(FablePerformanceCounterValue*) = Frequency;
}

static void Require(bool condition, const char* message)
{
    if (!condition) { printf("STOPWATCH FAIL %s\n", message); exit(1); }
}
static void Seconds(const CStopWatch& watch, float expected, const char* message)
{
    float delta = watch.GetElapsedSeconds() - expected;
    Require(delta < 0.00001f && delta > -0.00001f, message);
}
int main()
{
    Require(sizeof(CStopWatch) == 32 && offsetof(CStopWatch, m_nStartTick) == 8 &&
        offsetof(CStopWatch, m_nPrevElapsedTicks) == 16 &&
        offsetof(CStopWatch, m_bIsRunning) == 24, "PDB and retail layout");
    CStopWatch watch;
    Require(watch.m_fTimerPeriod == 0.01f && !watch.m_bIsRunning &&
        !watch.m_nStartTick && !watch.m_nPrevElapsedTicks, "construction");
    Seconds(watch, 0, "initial elapsed");
    watch.Stop();
    watch.Reset();
    Require(!counterCalls && !watch.m_bIsRunning, "stopped operations do not sample");
    watch.m_nStartTick = 123;
    watch.Reset();
    Require(watch.m_nStartTick == 123, "stopped Reset preserves start field");

    tick = 1000;
    watch.Start();
    tick = 1250;
    Seconds(watch, 2.5f, "running elapsed");
    Require(!watch.m_nPrevElapsedTicks && watch.m_nStartTick == 1000,
        "elapsed query does not mutate clock state");
    watch.Stop();
    Require(!watch.m_bIsRunning && !watch.m_nStartTick &&
        watch.m_nPrevElapsedTicks == 250, "stop accumulation");
    unsigned calls = counterCalls;
    tick = 9000;
    watch.Stop();
    Seconds(watch, 2.5f, "stopped clock frozen");
    Require(counterCalls == calls, "repeated stop and stopped elapsed do not sample");

    tick = 2000;
    watch.Start();
    tick = 2100;
    Seconds(watch, 3.5f, "resume retains accumulated time");
    watch.Start();
    tick = 2200;
    Seconds(watch, 3.5f, "repeated Start rebases uncommitted interval");
    watch.Reset();
    Require(watch.m_bIsRunning && watch.m_nStartTick == 2200 &&
        !watch.m_nPrevElapsedTicks, "running reset");
    tick = 2250;
    Seconds(watch, 0.5f, "elapsed after reset");
    watch.Stop();
    watch.Reset();
    Seconds(watch, 0, "stopped reset");
    Require(!watch.m_bIsRunning, "reset does not start stopped clock");

    watch.m_nPrevElapsedTicks = 999;
    tick = 3000;
    watch.StartZero();
    Require(!watch.m_nPrevElapsedTicks && watch.m_bIsRunning &&
        watch.m_nStartTick == 3000, "StartZero discards previous time");
    tick = 3100;
    watch.StartZero();
    Seconds(watch, 0, "StartZero while running");
    tick = 3150;
    watch.Stop();
    Seconds(watch, 0.5f, "Stop after StartZero");

    tick = (static_cast<__int64>(1) << 40) + 123;
    Require(watch.GetTicks() == tick, "counter retains high 32 bits");
    watch.StartZero();
    tick += 125;
    Seconds(watch, 1.25f, "large absolute counter");
    watch.Stop();
    Require(watch.m_nPrevElapsedTicks == 125, "64-bit subtraction");
    tick = (static_cast<__int64>(1) << 32) - 50;
    watch.StartZero();
    tick += 125;
    watch.Stop();
    Require(watch.m_nPrevElapsedTicks == 125, "subtraction across low-word rollover");
    watch.m_nPrevElapsedTicks = (static_cast<__int64>(1) << 32) - 50;
    watch.Start();
    tick += 125;
    watch.Stop();
    Require(watch.m_nPrevElapsedTicks == (static_cast<__int64>(1) << 32) + 75,
        "accumulation carries into high word");

    counterWorks = false;
    Require(watch.GetTicks() == 0, "failed counter query");
    watch.StartZero();
    Require(watch.m_bIsRunning && !watch.m_nStartTick, "failed start still starts");
    Seconds(watch, 0, "failed elapsed from zero");
    counterWorks = true;
    tick = 100;
    watch.StartZero();
    counterWorks = false;
    Seconds(watch, -1, "failure while running is not clamped");
    watch.Stop();
    Require(watch.m_nPrevElapsedTicks == -100 && !watch.m_bIsRunning,
        "failed Stop accumulates negative delta");
    watch.Start();
    Require(watch.m_nStartTick == 0 && watch.m_nPrevElapsedTicks == -100,
        "failed resume preserves accumulated ticks");
    watch.Reset();
    Require(!watch.m_nStartTick && !watch.m_nPrevElapsedTicks && watch.m_bIsRunning,
        "failed running reset");

    frequencyWorks = false;
    CStopWatch fallback;
    Require(fallback.m_fTimerPeriod == 1, "frequency failure fallback");
    counterWorks = true;
    tick = 5;
    fallback.StartZero();
    tick = 7;
    Seconds(fallback, 2, "fallback ticks interpreted as seconds");
    frequencyWorks = true;
    frequency = static_cast<__int64>(1) << 40;
    CStopWatch highFrequency;
    Require(highFrequency.m_fTimerPeriod == 1.0f / 1099511627776.0f,
        "64-bit frequency conversion");
    puts("FABLETLC_STOPWATCH_BEHAVIOR PASS");
    return 0;
}
