#pragma once
#include "rebuild_abi.h"

extern "C" __declspec(dllimport) int FABLE_STDCALL QueryPerformanceCounter(__int64* ticks);
extern "C" __declspec(dllimport) int FABLE_STDCALL QueryPerformanceFrequency(__int64* frequency);

// Retail/PDB layout, including alignment gaps at +4 and +0x19.
class CStopWatch
{
public:
    CStopWatch();
    __int64 GetTicks() const;
    void Start();
    void StartZero();
    void Stop();
    void Reset();
    float GetElapsedSeconds() const;

    float m_fTimerPeriod;
    __int64 m_nStartTick;
    __int64 m_nPrevElapsedTicks;
    bool m_bIsRunning;
};

inline __int64 CStopWatch::GetTicks() const
{
    __int64 ticks;
    if (QueryPerformanceCounter(&ticks))
        return ticks;
    return 0;
}

FABLE_STATIC_ASSERT(sizeof(CStopWatch) == 0x20);
