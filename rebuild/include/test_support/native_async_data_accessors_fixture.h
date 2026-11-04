#pragma comment(lib, "kernel32.lib")
// Minimal Win32 declarations keep this fixture independent of SDK headers.
typedef unsigned long DWORD;
typedef void* HANDLE;
#define WINAPI __stdcall
#define TRUE 1
#define FALSE 0
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT 258
struct _RTL_CRITICAL_SECTION {
    void* DebugInfo; long LockCount,RecursionCount;
    HANDLE OwningThread,LockSemaphore; unsigned long SpinCount;
};
typedef _RTL_CRITICAL_SECTION CRITICAL_SECTION;
extern "C" {
__declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) HANDLE __stdcall CreateEventA(void*,int,int,const char*);
__declspec(dllimport) int __stdcall SetEvent(HANDLE);
__declspec(dllimport) DWORD __stdcall WaitForSingleObject(HANDLE,DWORD);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE);
__declspec(dllimport) HANDLE __stdcall CreateThread(void*,unsigned long,DWORD (__stdcall*)(void*),void*,DWORD,DWORD*);
}
#define CreateEvent CreateEventA
#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
struct Probe { CBankFileAsyncData* data; HANDLE started,done; bool result; };
static DWORD WINAPI CheckOnWorker(void* argument) {
    Probe& probe=*static_cast<Probe*>(argument);
    SetEvent(probe.started);
    probe.result=probe.data->IsFinished();
    SetEvent(probe.done);
    return 0;
}
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d\n",__LINE__); return 1; } } while(0)
int main() {
    unsigned long bankBytes[sizeof(CBankFileAsync)/4];
    unsigned long dataBytes[sizeof(CBankFileAsyncData)/4];
    memset(bankBytes,0,sizeof(bankBytes)); memset(dataBytes,0,sizeof(dataBytes));
    CBankFileAsync& bank=*reinterpret_cast<CBankFileAsync*>(bankBytes);
    CBankFileAsyncData& data=*reinterpret_cast<CBankFileAsyncData*>(dataBytes);
    // Install the reference member's storage slot without constructing an I/O request.
    dataBytes[0x28/4]=reinterpret_cast<unsigned long>(&bank);
    CRITICAL_SECTION& section=*reinterpret_cast<CRITICAL_SECTION*>(bank.CriticalSection);
    InitializeCriticalSection(&section);
    data.AllowRestart=true;
    CHECK(!data.IsFinished() && data.AllowRestart);
    data.Finished=true;
    CHECK(data.IsFinished() && !data.AllowRestart);
    CHECK(data.IsFinished() && !data.AllowRestart);
    data.InternalAllocatedBufferSize=0; CHECK(data.Sizeof()==72);
    data.InternalAllocatedBufferSize=100; CHECK(data.Sizeof()==172);
    data.InternalAllocatedBufferSize=0xffffffc0; CHECK(data.Sizeof()==8);
    Probe probe; probe.data=&data; probe.result=false;
    probe.started=CreateEvent(0,TRUE,FALSE,0); probe.done=CreateEvent(0,TRUE,FALSE,0);
    CHECK(probe.started && probe.done);
    EnterCriticalSection(&section);
    data.Finished=false; data.AllowRestart=true;
    HANDLE worker=CreateThread(0,0,CheckOnWorker,&probe,0,0); CHECK(worker);
    CHECK(WaitForSingleObject(probe.started,5000)==WAIT_OBJECT_0);
    CHECK(WaitForSingleObject(probe.done,50)==WAIT_TIMEOUT);
    data.Finished=true;
    LeaveCriticalSection(&section);
    CHECK(WaitForSingleObject(probe.done,5000)==WAIT_OBJECT_0);
    CHECK(probe.result && !data.AllowRestart);
    CHECK(WaitForSingleObject(worker,5000)==WAIT_OBJECT_0);
    CloseHandle(worker); CloseHandle(probe.started); CloseHandle(probe.done);
    DeleteCriticalSection(&section);
    puts("ASYNC_DATA_ACCESSORS_PASS pending=1 finished=2 restartDisabled=1 nativeLockContention=1 sizeCases=3");
    return 0;
}

#include "../../src/compiled/00/a6/CCriticalSectionLock_Constructor_00a62920.cpp"

#include "../../src/compiled/00/a6/CCriticalSectionLock_Destructor_00a629a0.cpp"
