union CPerformanceCounterValue {
    struct { unsigned long LowPart; long HighPart; };
    __int64 QuadPart;
};
__declspec(dllimport) int __stdcall QueryPerformanceCounter(CPerformanceCounterValue* value);
struct CStopWatch { __int64 GetTicks() const; };
__int64 CStopWatch::GetTicks() const
{
    CPerformanceCounterValue ticks;
    if (QueryPerformanceCounter(&ticks))
        return ticks.QuadPart;
    return 0;
}