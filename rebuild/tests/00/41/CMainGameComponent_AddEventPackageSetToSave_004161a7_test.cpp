// Behaviour fixture for CMainGameComponent::AddEventPackageSetToSave @ 0x004161A7.
// EventPackageFile (+0x16154) gets a fake vptr with slot 4 (+0x10) Write.
// Checks: the scratch buffer is locked first; when the lock fails nothing
// else happens; otherwise the set is compressed into the scratch buffer, the
// returned size is written as a 4-byte length and then that many scratch
// bytes are written (null process both times), and the buffer is unlocked.
// The component is untouched.
#include "fable_game.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c, const CGameEventPackageSet* s) { static_cast<MainGameProbe*>(c)->AddEventPackageSetToSave(s); }
};

#include "test_support/scratch_buffer_storage.h"

static unsigned char comp[0x161F0];
static unsigned char packageSet[64];
static unsigned char scratch[256];
static void* fileVtbl[20];
static int seq, lockAt, lockCalls, writeAt[4], writeCalls, compressAt, compressCalls, unlockAt, unlockCalls, badCalls;
static bool lockResult, ok;

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
long CGameEventPackageSet::CompressIntoBuffer(unsigned char* buffer) const
{
    ok = ok && static_cast<const void*>(this) == packageSet && buffer == scratch;
    compressAt = ++seq; ++compressCalls;
    return 91;
}
static void __fastcall FakeWrite(void* self, void*, const void* data, long size, CASuspendableProcess* process)
{
    ok = ok && self == comp + 0x16154 && process == 0;
    if (writeCalls == 0)
        ok = ok && size == 4 && data != scratch && *static_cast<const long*>(data) == 91;
    else if (writeCalls == 1)
        ok = ok && size == 91 && data == scratch;
    if (writeCalls < 4) writeAt[writeCalls] = ++seq;
    ++writeCalls;
}
static void __fastcall FakeBad(void*, void*) { ++badCalls; }

static bool Run(bool lockOk)
{
    for (int i = 0; i < 20; ++i) fileVtbl[i] = (void*)FakeBad;
    fileVtbl[4] = (void*)FakeWrite;
    memset(comp, 0x2E, sizeof(comp));
    void* vt = fileVtbl;
    memcpy(comp + 0x16154, &vt, 4);
    static unsigned char before[sizeof(comp)];
    memcpy(before, comp, sizeof(comp));
    lockResult = lockOk;
    seq = lockAt = lockCalls = writeCalls = compressAt = compressCalls = unlockAt = unlockCalls = badCalls = 0;
    memset(writeAt, 0, sizeof(writeAt));
    ok = true;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp),
        reinterpret_cast<const CGameEventPackageSet*>(packageSet));
    ok = ok && lockAt == 1 && lockCalls == 1 && badCalls == 0 && memcmp(before, comp, sizeof(comp)) == 0;
    if (lockOk)
        ok = ok && compressCalls == 1 && compressAt == 2 && writeCalls == 2 && writeAt[0] == 3
            && writeAt[1] == 4 && unlockCalls == 1 && unlockAt == 5;
    else
        ok = ok && compressCalls == 0 && writeCalls == 0 && unlockCalls == 0;
    if (!ok) printf("case lock=%d failed\n", lockOk);
    return ok;
}

int main()
{
    bool pass = Run(true);
    pass = Run(false) && pass;
    printf(pass ? "PASS_004161a7\n" : "FAIL_004161a7\n");
    return pass ? 0 : 1;
}
