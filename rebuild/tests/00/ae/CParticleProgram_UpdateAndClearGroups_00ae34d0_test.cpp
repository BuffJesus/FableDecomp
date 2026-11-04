// Behaviour fixture for CParticleProgram::Update and ClearGroups (provider CParticleProgram_UpdateAndClearGroups_00ae34d0.cpp).
// The program is raw storage at retail offsets (exports +0x88 with compare byte +0x94 and dirty flag +0x95, SimGroups
// +0xFC, NonSimGroups +0x108). The fixture supplies the export sort (recording its range), CalculateAveragePatchSize
// (recording its argument) and the blend-vector assignment and tail copy the group erase could use (must move nothing). Checked: Update
// sorts the whole export list exactly when the list is dirty, clears the flag, and in every case recalculates the
// average patch size from the initial simulation (program + 0x08) after the sort; ClearGroups empties both group lists
// (each group destroyed in place, nothing moved) and leaves the rest of the program alone.
#include <stdio.h>
#include <string.h>
#include "fable_particle_program.h"

static int g_fail, g_step, g_sorts, g_sortStep, g_patches, g_patchStep;
static const void *g_first, *g_last, *g_patchArg, *g_patchThis;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

typedef _STL::pair<CCharString, unsigned long> Export;
CCharString::~CCharString() {}
namespace _STL
{
template <> void FABLE_FASTCALL sort<Export*, CKeyPairCompareLess<CCharString, unsigned long> >(Export* first, Export* last, CKeyPairCompareLess<CCharString, unsigned long>)
{
    ++g_sorts; g_sortStep = ++g_step; g_first = first; g_last = last;
}
// Erasing a whole list moves nothing: the tail copy runs over an empty range.
template <> C3DGroup2* FABLE_FASTCALL __copy<C3DGroup2*, C3DGroup2*, int>(C3DGroup2* first, C3DGroup2* last, C3DGroup2* result, const random_access_iterator_tag&, int*)
{
    CHECK(first == last); return result;
}
template <> vector<C3DVertexBlend2, allocator<C3DVertexBlend2> >& vector<C3DVertexBlend2, allocator<C3DVertexBlend2> >::operator=(const vector<C3DVertexBlend2, allocator<C3DVertexBlend2> >&)
{
    CHECK(!"no group is moved when the whole list is erased"); return *this;
}
}
void CParticleProgram::CalculateAveragePatchSize(const CParticleSimulation& simulation)
{
    ++g_patches; g_patchStep = ++g_step; g_patchArg = &simulation; g_patchThis = this;
}

static unsigned char raw[0x140], before[0x140];
static void SetVector(unsigned offset, void* begin, unsigned count, unsigned elementSize)
{
    unsigned char* b = (unsigned char*)begin; unsigned char* e = b + count * elementSize;
    memcpy(raw + offset, &b, 4); memcpy(raw + offset + 4, &e, 4); memcpy(raw + offset + 8, &e, 4);
}
static bool Empty(unsigned offset) { return memcmp(raw + offset, raw + offset + 4, 4) == 0; }

int main()
{
    static unsigned long names[3][2];
    static unsigned long groups[2][4], nonSimGroups[3][4];   // C3DGroup2 with empty blend vectors
    for (int i = 0; i < 0x140; ++i) raw[i] = (unsigned char)(0x21 + i);
    SetVector(0x88, names, 3, 8);
    CParticleProgram* program = reinterpret_cast<CParticleProgram*>(raw);

    // Clean list: no sort, patch size still recalculated.
    raw[0x95] = 0;
    memcpy(before, raw, sizeof(raw));
    program->Update();
    CHECK(g_sorts == 0 && g_patches == 1 && g_patchArg == raw + 0x08 && g_patchThis == raw);
    CHECK(memcmp(raw, before, sizeof(raw)) == 0);

    // Dirty list: sorted over [begin, end), flag cleared, then the patch size.
    raw[0x95] = 1;
    program->Update();
    CHECK(g_sorts == 1 && g_first == (void*)names && g_last == (void*)(names + 3));
    CHECK(raw[0x95] == 0 && g_patches == 2 && g_sortStep < g_patchStep && g_patchArg == raw + 0x08);
    raw[0x95] = 1; memcpy(before, raw, sizeof(raw)); before[0x95] = 0;
    program->Update();
    CHECK(g_sorts == 2 && memcmp(raw, before, sizeof(raw)) == 0);

    // ClearGroups.
    SetVector(0xFC, groups, 2, 16); SetVector(0x108, nonSimGroups, 3, 16);
    memcpy(before, raw, sizeof(raw));
    program->ClearGroups();
    CHECK(Empty(0xFC) && Empty(0x108));
    CHECK(memcmp(raw + 0xFC, before + 0xFC, 4) == 0 && memcmp(raw + 0x108, before + 0x108, 4) == 0);   // storage kept
    CHECK(memcmp(raw + 0xFC + 8, before + 0xFC + 8, 4) == 0 && memcmp(raw + 0x108 + 8, before + 0x108 + 8, 4) == 0);
    CHECK(memcmp(raw, before, 0xFC) == 0 && memcmp(raw + 0x114, before + 0x114, 0x140 - 0x114) == 0);
    CHECK(g_sorts == 2 && g_patches == 3);

    if (g_fail) { printf("UPDATE_CLEAR_GROUPS_FAIL %d\n", g_fail); return 1; }
    printf("UPDATE_CLEAR_GROUPS_OK\n");
    return 0;
}
