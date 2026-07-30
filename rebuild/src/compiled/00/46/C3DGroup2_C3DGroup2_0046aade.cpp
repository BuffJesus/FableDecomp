#pragma optimize("s",on)
struct T3DGroup2 {
    int f0;
    int f4;
    int f8;
    int fc;
};

// Constructor for C3DGroup2 at 0x0046aade (__fastcall -> __fastcall under VC7.1).
// Sets [this]=0, [this+4]=-1, [this+8]=-1, [this+0xc]=-1, returns this in eax.
T3DGroup2* __fastcall C3DGroup2(T3DGroup2* pThis) {
    pThis->f0 = 0;
    pThis->f4 = -1;
    pThis->f8 = -1;
    pThis->fc = -1;
    return pThis;
}