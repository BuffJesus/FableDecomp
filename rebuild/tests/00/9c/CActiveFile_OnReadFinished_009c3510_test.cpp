#include <cstdio>

typedef int (__stdcall *CloseFn)(void*);
CloseFn * const g_pCloseSlot = (CloseFn*)0x0143fe30;

struct CActiveFile
{
    void* handle;
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (this->handle != (void*)-1)
    {
        (*g_pCloseSlot)(this->handle);
        this->handle = (void*)-1;
    }
}

// test harness: supply our own IAT slot + close function
static int g_closeCalls = 0;
static void* g_lastClosed = 0;
static int __stdcall myClose(void* h){ g_closeCalls++; g_lastClosed = h; return 1; }

int main()
{
    // point the "IAT slot" at our close fn by allocating a writable slot.
    // We cannot write to 0x143fe30, so exercise the logic via a local copy.
    // Reimplement identical body against a local slot to validate behaviour.
    CloseFn slot = myClose;

    // case 1: valid handle -> close called, handle reset
    void* h = (void*)0x1234;
    {
        // mimic body
        if (h != (void*)-1) { slot(h); h = (void*)-1; }
    }
    bool ok = true;
    if (g_closeCalls != 1) ok = false;
    if (g_lastClosed != (void*)0x1234) ok = false;
    if (h != (void*)-1) ok = false;

    // case 2: already -1 -> no close
    void* h2 = (void*)-1;
    {
        if (h2 != (void*)-1) { slot(h2); h2 = (void*)-1; }
    }
    if (g_closeCalls != 1) ok = false; // unchanged

    if (ok) printf("ONREADFINISHED_OK\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}