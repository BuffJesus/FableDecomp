// CTCGatherPointBuilding::Init  @ 0x00c46eb0  (__fastcall, ret 4)
// Copies two fields from *this into an out-struct passed by hidden pointer,
// then zeroes the third. Retail copies f4 (float) via the x87 stack (fld/fstp)
// and f0 as a raw 32-bit int; both source fields are read before the out
// pointer is loaded. The volatile-qualified float member forces VC7.1 /O2 to
// emit fld/fstp instead of an integer move, and the local temps force both
// self reads to precede the out-pointer load, reproducing the exact byte
// sequence: d9 41 04 8b 09 8b 44 24 04 d9 58 04 89 08 c7 40 08 00000000 c2 04 00
struct GPBOut {
    int   f0;   // +0
    float f4;   // +4
    int   f8;   // +8
};

struct CTCGatherPointBuilding {
    int            f0;   // +0
    volatile float f4;   // +4
    int            f8;   // +8
    void Init(GPBOut *out);
};

void CTCGatherPointBuilding::Init(GPBOut *out)
{
    float t = this->f4;
    int   u = this->f0;
    out->f4 = t;
    out->f0 = u;
    out->f8 = 0;
}