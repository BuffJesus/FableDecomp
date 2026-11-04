// Behaviour fixture for trivial virtual defaults re-landed as genuine members
// (tools/decomp_pipeline/vtable_defaults.py). Each member is the real compiled
// provider, called non-virtually on scratch storage with dummy arguments; it
// must return its constant and leave the object and the arguments untouched.
#include "fable_game_text_bank.h"
#include "fable_iengine.h"
#include "fable_memory_allocator.h"
#include "fable_progress_display.h"
#include <stdio.h>
#include <string.h>

static union { double align; unsigned char bytes[4096]; } g_object, g_objectBefore;
static union { double align; unsigned char bytes[4096]; } g_scratch, g_scratchBefore;
static int g_fail;

int main()
{
    int cases = 0;
    for (int seed = 0; seed < 3; ++seed)
    {
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 0); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<CIEngine*>(&g_object)->CIEngine::GetStaticMapUpdatePassCount() == 0)) { printf("VTABLE_DEFAULTS_FAIL 0 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 0 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 1); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<const CIEngine*>(&g_object)->CIEngine::IsPreloaded() == true)) { printf("VTABLE_DEFAULTS_FAIL 1 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 1 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 2); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<CIEngine*>(&g_object)->CIEngine::GetLoadingMemoryPool() == 0)) { printf("VTABLE_DEFAULTS_FAIL 2 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 2 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 3); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<CIEngine*>(&g_object)->CIEngine::GetAnimationMemoryPool() == 0)) { printf("VTABLE_DEFAULTS_FAIL 3 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 3 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 4); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CAProgressDisplay*>(&g_object)->CAProgressDisplay::InitialiseTextures(seed != 0, seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 4 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 5); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::PausedWorldFrameUpdate();
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 5 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 6); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::Refresh();
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 6 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 7); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::CloseStaticMapFile();
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 7 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 8); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::ReloadLocalDetailDefs();
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 8 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 9); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::ChangeCameraAngle();
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 9 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 10); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::ReallocateShadowBuffer();
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 10 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 11); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetEnvironmentalEffectsAlwaysUpdate(seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 11 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 12); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetWorld((CIDrawWorld const *)0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 12 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 13); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::WorldFrameUpdate(seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 13 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 14); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::AddPrimitive(*reinterpret_cast<CEnginePrimitive const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 14 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 15); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetPrimitiveAnimation(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), *reinterpret_cast<CEnginePrimitiveAnimation const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 15 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 16); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetPrimitivePosition(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), *reinterpret_cast<CEnginePrimitivePosition const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 16 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 17); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetChildPrimitive(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), (unsigned long)(seed + 7), *reinterpret_cast<CEnginePrimitive const*>(&g_scratch), (unsigned long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 17 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 18); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetChildPrimitive(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), (unsigned long)(seed + 7), *reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), *reinterpret_cast<CEnginePrimitive const*>(&g_scratch), (unsigned long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 18 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 19); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::GetChildPrimitiveHandle(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), (unsigned long)(seed + 7), *reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 19 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 20); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::ClearChildPrimitive(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), (unsigned long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 20 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 21); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::ClearAllChildPrimitives(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 21 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 22); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetAttachedPrimitive(*reinterpret_cast<CEnginePrimitiveHandle*>(&g_scratch), *reinterpret_cast<CEnginePrimitive const*>(&g_scratch), *reinterpret_cast<CEnginePrimitiveAttachmentInfo const*>(&g_scratch), *reinterpret_cast<CEnginePrimitiveHandle const*>(&g_scratch), (long)(seed + 7), (unsigned long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 22 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 23); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::StartStaticMapUpdatePass(*reinterpret_cast<CWideString const*>(&g_scratch), (long)(seed + 7), *reinterpret_cast<CStaticMapGenerationVisibiilityInfo const*>(&g_scratch), *reinterpret_cast<CArray<CCharString> const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 23 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 24); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::UpdateStaticMap((long)(seed + 7), *reinterpret_cast<CCharString const*>(&g_scratch), (unsigned long)(seed + 7), (long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 24 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 25); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::AddMap((long)(seed + 7), (long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 25 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 26); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::RemoveMap((long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 26 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 27); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetMapAsLoaded((long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 27 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 28); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetMapAsUnloaded((long)(seed + 7));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 28 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 29); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetMapVisibilityAllMaps(seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 29 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 30); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetMapVisibility((long)(seed + 7), seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 30 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 31); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::AreaChanged(*reinterpret_cast<C2DBoxI const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 31 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 32); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetStaticMapFileForUse(*reinterpret_cast<CWideString const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 32 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 33); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::StartNewScene((unsigned long)(seed + 7), 0.5f * seed);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 33 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 34); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::CutscenePreload(seed != 0, seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 34 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 35); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::CutsceneAddCameraAngle(*reinterpret_cast<CCamera const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 35 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 36); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetScreenColourFilter(*reinterpret_cast<CEngineColourFilterSettings const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 36 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 37); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetSpotlightShadowMode(seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 37 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 38); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::UpdateDebugOverlay((long)(seed + 7), (CRGBColour const *)0, *reinterpret_cast<C2DBoxI const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 38 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 39); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::EnableDebugOverlay((long)(seed + 7), seed != 0, seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 39 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 40); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetGlobalLightingParameters(*reinterpret_cast<C3DVector const*>(&g_scratch), *reinterpret_cast<CRGBFloatColour const*>(&g_scratch), *reinterpret_cast<CRGBFloatColour const*>(&g_scratch), *reinterpret_cast<CRGBFloatColour const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 40 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 41); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::Set2DPrimitiveLightingParameters(*reinterpret_cast<C3DVector const*>(&g_scratch), *reinterpret_cast<CRGBFloatColour const*>(&g_scratch), *reinterpret_cast<CRGBFloatColour const*>(&g_scratch), *reinterpret_cast<CRGBFloatColour const*>(&g_scratch));
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 41 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 42); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::SetRenderTargetTexture((CTexture const *)0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 42 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 43); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::FreeTextureFromEngineResourcePools((CTexture *)0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 43 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 44); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<CIEngine*>(&g_object)->CIEngine::GetScratchBufferTextureManager() == 0)) { printf("VTABLE_DEFAULTS_FAIL 44 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 44 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 45); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::Enable3DSceneRendering(seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 45 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 46); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<CIEngine*>(&g_object)->CIEngine::Is3DSceneRenderingEnabled() == true)) { printf("VTABLE_DEFAULTS_FAIL 46 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 46 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 47); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::Enable2DSceneRendering(seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 47 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 48); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<CIEngine*>(&g_object)->CIEngine::Is2DSceneRenderingEnabled() == true)) { printf("VTABLE_DEFAULTS_FAIL 48 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 48 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 49); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::OverrideRain(0.5f * seed);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 49 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 50); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::OverrideSnow(0.5f * seed);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 50 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 51); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CIEngine*>(&g_object)->CIEngine::RenderColorFilteredQuad(0.5f * seed, 0.5f * seed, *reinterpret_cast<C4DVector const*>(&g_scratch), 0.5f * seed, 0.5f * seed, (CTexture *)0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 51 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 52); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CAProgressDisplay*>(&g_object)->CAProgressDisplay::SetToDisplayText(seed != 0);
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 52 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 53); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        if (!(reinterpret_cast<const CMovableResource*>(&g_object)->CMovableResource::Verify() == 0x7b)) { printf("VTABLE_DEFAULTS_FAIL 53 value\n"); g_fail = 1; }
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 53 state\n"); g_fail = 1; }
        ++cases;
        for (unsigned i = 0; i < sizeof(g_object.bytes); ++i) { g_object.bytes[i] = (unsigned char)(i * 7 + seed + 54); g_scratch.bytes[i] = (unsigned char)(i * 3 + seed); }
        g_objectBefore = g_object; g_scratchBefore = g_scratch;
        reinterpret_cast<CMemoryAllocatorBase*>(&g_object)->CMemoryAllocatorBase::ConsolidateFreeAreas();
        if (memcmp(&g_object, &g_objectBefore, sizeof(g_object)) || memcmp(&g_scratch, &g_scratchBefore, sizeof(g_scratch))) { printf("VTABLE_DEFAULTS_FAIL 54 state\n"); g_fail = 1; }
        ++cases;
    }
    if (g_fail) return 1;
    printf("VTABLE_DEFAULTS_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
