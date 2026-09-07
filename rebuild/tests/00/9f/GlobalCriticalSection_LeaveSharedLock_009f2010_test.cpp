#include <stdio.h>
struct FableCriticalSection {
    void* DebugInfo;
    long LockCount;
    long RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};
// Named differently from the real import so the CRT's kernel32 import stub cannot
// collide at link time; the shape under test is "pass the global's address through".
static FableCriticalSection* g_lastLeft = 0;
extern "C" void __stdcall LeaveCriticalSectionProbe(FableCriticalSection* p) { g_lastLeft = p; }
FableCriticalSection g_fableSharedLock;
void LeaveSharedLock() { LeaveCriticalSectionProbe(&g_fableSharedLock); }
int main() {
    LeaveSharedLock();
    if (g_lastLeft != &g_fableSharedLock) return 1;
    printf("LEAVE_SHARED_LOCK PASS\n");
    return 0;
}