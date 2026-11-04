// Behaviour fixture for CMainGameComponent::UninitEventPackageSaving @ 0x00416122.
// EventPackageFile (+0x16154) is given a fake CDiskFileWin32 vptr: slot 10
// (+0x28) IsOpen, slot 2 (+0x08) Close. Checks: IsOpen is asked once on the
// file; Close runs on it only when open (after IsOpen); SavingEventPackages
// (+0x16171) is cleared either way; LoadingEventPackages (+0x16170) and every
// other byte are untouched.
#include "fable_game.h"
#include <stdio.h>
#include <string.h>

struct MainGameProbe : CMainGameComponent
{
    static void Call(CMainGameComponent* c) { static_cast<MainGameProbe*>(c)->UninitEventPackageSaving(); }
};

static unsigned char comp[0x161F0];
static void* fileVtbl[20];
static int seq, isOpenAt, isOpenCalls, closeAt, closeCalls, badCalls;
static bool fileOpen, ok;

static bool __fastcall FakeIsOpen(void* self, void*)
{
    ok = ok && self == comp + 0x16154;
    isOpenAt = ++seq; ++isOpenCalls;
    return fileOpen;
}
static void __fastcall FakeClose(void* self, void*)
{
    ok = ok && self == comp + 0x16154 && comp[0x16171] == 1;
    closeAt = ++seq; ++closeCalls;
}
static void __fastcall FakeBad(void*, void*) { ++badCalls; }

static bool Run(bool open)
{
    for (int i = 0; i < 20; ++i) fileVtbl[i] = (void*)FakeBad;
    fileVtbl[10] = (void*)FakeIsOpen;
    fileVtbl[2] = (void*)FakeClose;
    memset(comp, 0xC3, sizeof(comp));
    void* vt = fileVtbl;
    memcpy(comp + 0x16154, &vt, 4);
    comp[0x16170] = 1;
    comp[0x16171] = 1;
    static unsigned char expect[sizeof(comp)];
    memcpy(expect, comp, sizeof(comp));
    expect[0x16171] = 0;
    fileOpen = open;
    seq = isOpenAt = isOpenCalls = closeAt = closeCalls = badCalls = 0;
    ok = true;
    MainGameProbe::Call(reinterpret_cast<CMainGameComponent*>(comp));
    ok = ok && isOpenCalls == 1 && isOpenAt == 1 && badCalls == 0
        && closeCalls == (open ? 1 : 0) && (!open || closeAt == 2)
        && memcmp(expect, comp, sizeof(comp)) == 0;
    if (!ok) printf("case open=%d failed\n", open);
    return ok;
}

int main()
{
    bool pass = Run(true);
    pass = Run(false) && pass;
    printf(pass ? "PASS_00416122\n" : "FAIL_00416122\n");
    return pass ? 0 : 1;
}
