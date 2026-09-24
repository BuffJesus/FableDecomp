#pragma once

// Win32 LARGE_INTEGER and the two kernel32 imports used by CStopWatch.
// Keep the ABI available to the VC7.1 reconstruction without a platform SDK.
union FablePerformanceCounterValue
{
    __int64 QuadPart;
};
extern "C" {
    __declspec(dllimport) int __stdcall QueryPerformanceCounter(FablePerformanceCounterValue*);
    __declspec(dllimport) int __stdcall QueryPerformanceFrequency(FablePerformanceCounterValue*);
}
