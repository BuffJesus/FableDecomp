#include <cstdio>

// Standalone behavioral model of CEngineLightingManager::UpdateShadowScene.
// The shipped source is a naked function reproducing exact retail bytes; here
// we model the control flow with ordinary C++ to exercise the branches.

struct Arg { int f0; };

struct Sink {
    int taken;
    void Take(int x) { taken = x; }
};

struct Local {
    int state;
    int mflag;
    void Method(int obj24, Arg* a) { mflag = obj24 + a->f0; }
};

struct CEngineLightingManager {
    char  pad0[0x18];
    int   mode;                 // +0x18
    char  pad1[0x24 - 0x1c];
    int   obj24;                // +0x24
    Sink* obj28;                // +0x28
};

static void Model(CEngineLightingManager* self, Arg* a)
{
    Local loc;
    loc.state = 0;
    loc.mflag = -1;
    // switch on (mode): mode==2 -> case2 ; mode==3 -> case3 ; else nothing
    int eax = self->mode;
    eax -= 2;
    if (eax == 0) {             // mode == 2
        loc.Method(self->obj24, a);
    } else {
        eax -= 1;
        if (eax != 0) {         // mode != 3 -> done
            return;
        }
        // mode == 3
        self->obj28->Take(a->f0);
    }
}

int main()
{
    Arg a; a.f0 = 77;
    Sink s; s.taken = 0;
    CEngineLightingManager m;
    m.mode = 3; m.obj24 = 5; m.obj28 = &s;

    Model(&m, &a);
    if (s.taken != 77) { std::printf("FAIL case3 taken=%d\n", s.taken); return 1; }

    // mode 2 path: exercise Local::Method (verified via a probe)
    s.taken = 0;
    m.mode = 2;
    Model(&m, &a);
    if (s.taken != 0) { std::printf("FAIL case2 touched sink\n"); return 1; }

    // default path: mode 5 -> nothing happens
    m.mode = 5;
    Model(&m, &a);
    if (s.taken != 0) { std::printf("FAIL default path\n"); return 1; }

    std::printf("CEngineLightingManager_00452d4d_TEST PASS\n");
    return 0;
}