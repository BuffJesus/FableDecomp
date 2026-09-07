// Retail 0x009F2010. The manifest/BSim label for this row is
// `_Dest_val<std::allocator<NCreatureHitNotification::CFlash>,...>`, which the body
// contradicts: it is a bare `push offset g_lock; call [__imp_LeaveCriticalSection]; ret`.
// The IAT slot at 0x0143FE10 resolves to KERNEL32.dll!LeaveCriticalSection.
struct FableCriticalSection {
    void* DebugInfo;
    long LockCount;
    long RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(FableCriticalSection*);
extern FableCriticalSection g_fableSharedLock;

void LeaveSharedLock() {
    LeaveCriticalSection(&g_fableSharedLock);
}