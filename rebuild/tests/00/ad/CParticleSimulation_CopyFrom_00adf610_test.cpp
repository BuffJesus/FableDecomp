// Behaviour fixture for CParticleSimulation::CopyFrom and GetBoundingBox (provider CParticleSimulation_CopyFrom_00adf610.cpp).
// Both simulations are raw storage at retail offsets. The fixture supplies AllocateArrays (recording; it points the
// target's five arrays at fixture buffers, or fails). Checked: on failure only Timestep, TimestepChanged,
// TimestepMultiplier and Size are copied before false is returned; on success the four C4DVector arrays (16 bytes per
// particle) and the alphas (4 bytes per particle) are copied for exactly Size particles, gravity, wind, dragging,
// acceleration flag, damping, the double clock, the bounding box and its valid flag follow, MaximumAdjustment, the
// render data, memory handle and pool are left alone, and true is returned. GetBoundingBox returns the box and marks it
// valid without changing anything else.
#include <stdio.h>
#include <string.h>
#include "fable_particle_simulation.h"
#include "fable_4d_vector.h"

static int g_fail, g_allocs;
static bool g_ok;
static const void *g_allocThis, *g_allocPool;
static unsigned char g_arrays[4][8 * 16], g_alphas[8 * 4];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static void Put(unsigned char* raw, unsigned offset, const void* value) { memcpy(raw + offset, &value, 4); }
// Timestep +0x0C, TimestepChanged +0x10 (bool; +0x11..0x13 padding), TimestepMultiplier +0x14, Size +0x18.
static bool HeaderCopied(const unsigned char* d, const unsigned char* s)
{
    return memcmp(d + 0x0C, s + 0x0C, 4) == 0 && d[0x10] == s[0x10] && memcmp(d + 0x14, s + 0x14, 8) == 0;
}
static void PutWord(unsigned char* raw, unsigned offset, unsigned long value) { memcpy(raw + offset, &value, 4); }

bool CParticleSimulation::AllocateArrays(CMovableResourceMemoryPool& pool)
{
    ++g_allocs; g_allocThis = this; g_allocPool = &pool;
    unsigned char* raw = reinterpret_cast<unsigned char*>(this);
    if (!g_ok) return false;
    for (int i = 0; i < 4; ++i) Put(raw, 0x1C + 4 * i, g_arrays[i]);
    Put(raw, 0x2C, g_alphas);
    return true;
}

int main()
{
    static unsigned char src[0x80], dst[0x80], dstBefore[0x80], srcBefore[0x80], pool[0x40];
    static unsigned char srcArrays[4][8 * 16], srcAlphas[8 * 4];
    for (int i = 0; i < 0x80; ++i) src[i] = (unsigned char)(0x80 + i);
    for (int a = 0; a < 4; ++a) for (int i = 0; i < 8 * 16; ++i) srcArrays[a][i] = (unsigned char)(a * 40 + i + 1);
    for (int i = 0; i < 8 * 4; ++i) srcAlphas[i] = (unsigned char)(200 + i);
    PutWord(src, 0x18, 3);
    for (int a = 0; a < 4; ++a) Put(src, 0x1C + 4 * a, srcArrays[a]);
    Put(src, 0x2C, srcAlphas);
    double clock = 12.5; memcpy(src + 0x50, &clock, 8);
    memcpy(srcBefore, src, sizeof(src));
    CParticleSimulation& s = *reinterpret_cast<CParticleSimulation*>(src);
    CParticleSimulation& d = *reinterpret_cast<CParticleSimulation*>(dst);
    CMovableResourceMemoryPool& mp = *reinterpret_cast<CMovableResourceMemoryPool*>(pool);

    // Allocation fails: header only.
    memset(dst, 0x11, sizeof(dst)); memcpy(dstBefore, dst, sizeof(dst));
    g_ok = false;
    CHECK(!d.CopyFrom(s, mp));
    CHECK(g_allocs == 1 && g_allocThis == dst && g_allocPool == pool);
    CHECK(HeaderCopied(dst, src));
    CHECK(memcmp(dst, dstBefore, 0x0C) == 0 && memcmp(dst + 0x1C, dstBefore + 0x1C, 0x80 - 0x1C) == 0);
    CHECK(memcmp(dst + 0x11, dstBefore + 0x11, 3) == 0);

    // Success.
    memset(dst, 0x22, sizeof(dst)); memset(g_arrays, 0xEE, sizeof(g_arrays)); memset(g_alphas, 0xEE, sizeof(g_alphas));
    memcpy(dstBefore, dst, sizeof(dst));
    g_ok = true;
    CHECK(d.CopyFrom(s, mp));
    CHECK(g_allocs == 2);
    for (int a = 0; a < 4; ++a)
    {
        CHECK(memcmp(g_arrays[a], srcArrays[a], 3 * 16) == 0);
        for (int i = 3 * 16; i < 8 * 16; ++i) CHECK(g_arrays[a][i] == 0xEE);
    }
    CHECK(memcmp(g_alphas, srcAlphas, 3 * 4) == 0 && g_alphas[3 * 4] == 0xEE);
    CHECK(HeaderCopied(dst, src));
    CHECK(memcmp(dst + 0x30, dstBefore + 0x30, 4) == 0);                    // MaximumAdjustment not copied
    CHECK(memcmp(dst + 0x34, src + 0x34, 0x0A) == 0);                       // gravity, wind, dragging flags
    CHECK(memcmp(dst + 0x40, src + 0x40, 4) == 0 && dst[0x44] == src[0x44]); // dragging strength, acceleration flag
    CHECK(memcmp(dst + 0x48, src + 0x48, 4) == 0 && memcmp(dst + 0x50, &clock, 8) == 0);
    CHECK(memcmp(dst + 0x58, src + 0x58, 0x18) == 0 && dst[0x70] == src[0x70]);
    CHECK(memcmp(dst, dstBefore, 0x0C) == 0 && memcmp(dst + 0x74, dstBefore + 0x74, 0x0C) == 0);  // vtable/render data, handle, pool
    CHECK(memcmp(src, srcBefore, sizeof(src)) == 0);

    // GetBoundingBox.
    src[0x70] = 0;
    memcpy(srcBefore, src, sizeof(src));
    C3DBoundingBox box = s.GetBoundingBox();
    CHECK(memcmp(&box, src + 0x58, 0x18) == 0 && src[0x70] == 1);
    srcBefore[0x70] = 1;
    CHECK(memcmp(src, srcBefore, sizeof(src)) == 0);

    if (g_fail) { printf("COPY_FROM_FAIL %d\n", g_fail); return 1; }
    printf("COPY_FROM_OK\n");
    return 0;
}
