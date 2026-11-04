// Behaviour fixture for CMainGameComponent::GetEventPackageSetFromSave @ 0x00416148.
// EventPackageFile (+0x16154) gets a fake vptr with slot 3 (+0x0C) Read.
// Checks: the scratch buffer is locked first; when the lock fails nothing
// else happens and the result is false; otherwise the file is read twice (a
// 4-byte length into a local, then that many bytes into the scratch buffer,
// both with a null process), the set is rebuilt from the scratch buffer, the
// buffer is unlocked, and the result is true. The component is untouched.
#include "fable_game.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static bool Call(CMainGameComponent* c, CGameEventPackageSet* s) { return static_cast<MainGameProbe*>(c)->GetEventPackageSetFromSave(s); }
};

#include "test_support/scratch_buffer_storage.h"

static unsigned char comp[0x161F0];
static unsigned char packageSet[64];
static unsigned char scratch[256];
static void* fileVtbl[20];
static int seq, lockAt, lockCalls, readAt[4], readCalls, initAt, initCalls, unlockAt, unlockCalls, badCalls;
static bool lockResult, ok;
static void* lengthAddr;

bool CScratchBuffer::Lock(void** buffer)
{
    ok = ok && this == &GScratchBuffer;
    lockAt = ++seq; ++lockCalls;
    if (lockResult) *buffer = scratch;
    return lockResult;
}
void CScratchBuffer::Unlock()
{
    ok = ok && this == &GScratchBuffer;
    unlockAt = ++seq; ++unlockCalls;
}
void CGameEventPackageSet::InitFromCompressedBuffer(const unsigned char* buffer)
{
    ok = ok && static_cast<void*>(this) == packageSet && buffer == scratch;
    initAt = ++seq; ++initCalls;
}
static void __fastcall FakeRead(void* self, void*, void* data, long size, CASuspendableProcess* process)
{
    ok = ok && self == comp + 0x16154 && process == 0;
    if (readCalls == 0)
    {
        ok = ok && size == 4 && data != scratch;
        lengthAddr = data;
        *static_cast<long*>(data) = 37;
    }
    else if (readCalls == 1)
    {
        ok = ok && size == 37 && data == scratch;
    }
    if (readCalls < 4) readAt[readCalls] = ++seq;
    ++readCalls;
}
static void __fastcall FakeBad(void*, void*) { ++badCalls; }

static bool Run(bool lockOk)
{
    for (int i = 0; i < 20; ++i) fileVtbl[i] = (void*)FakeBad;
    fileVtbl[3] = (void*)FakeRead;
    memset(comp, 0x6B, sizeof(comp));
    void* vt = fileVtbl;
    memcpy(comp + 0x16154, &vt, 4);
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    lockResult = lockOk;
    seq = lockAt = lockCalls = readCalls = initAt = initCalls = unlockAt = unlockCalls = badCalls = 0;
    memset(readAt, 0, sizeof(readAt));
    ok = true;
    bool result = MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp),
        reinterpret_cast<CGameEventPackageSet*>(packageSet));
    ok = ok && lockAt == 1 && lockCalls == 1 && badCalls == 0 && memcmp(before, comp, sizeof(comp)) == 0;
    if (lockOk)
        ok = ok && result && readCalls == 2 && readAt[0] == 2 && readAt[1] == 3
            && initCalls == 1 && initAt == 4 && unlockCalls == 1 && unlockAt == 5;
    else
        ok = ok && !result && readCalls == 0 && initCalls == 0 && unlockCalls == 0;
    if (!ok) printf("case lock=%d failed\n", lockOk);
    return ok;
}

int main()
{
    bool pass = Run(true);
    pass = Run(false) && pass;
    printf(pass ? "PASS_00416148\n" : "FAIL_00416148\n");
    return pass ? 0 : 1;
}
