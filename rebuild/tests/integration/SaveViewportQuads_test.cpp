// Behavior fixture for the saved-games preview viewport (UI_VIEW_RING_SMALL).
//
// The saved-games screen draws a 256x256 region-minimap-plus-ring panel in the
// top-right (design (314,37)).  Reverse-engineering wired it as a live Render2D
// quad that reuses the already-attached g_OptionsTexture frontend atlas: the
// ring pixels are baked into each save cell of that atlas by
// tools/render_fable_frontend_subscreens.py, and FableRenderVisualD3D9 emits a
// quad sampling that rect AFTER the title rule so the panel draws on top,
// matching retail composite order.
//
// The live render path itself needs a Direct3D9 device, so this fixture guards
// the load-bearing, device-independent contract instead: the atlas UV rect the
// C++ ring quad samples (FableComputeSaveViewportAtlasRect) must coincide,
// pixel-for-pixel, with the atlas location the Python bake writes the panel to.
// If either side's geometry drifts, the ring quad binds the wrong pixels (or a
// neighbouring cell) and this fixture fails before the flat-white/regression
// shows up only in the runtime smoke image.
//
// SCOPE: this guards the atlas UV-rect contract only.  The live composite
// ORDER (ring emitted after the title rule) and texture BINDING (g_OptionsTexture
// non-null) live inside FableRenderVisualD3D9, which needs a D3D9 device; that
// ordering is covered end-to-end by the Python oracle=PIXEL_IDENTICAL zero-mask
// recomposition gate in tools/render_fable_frontend_subscreens.py, not here.

#include "../../integration/fable_visual_d3d9.h"

#include <stdio.h>

namespace
{
// Mirror of the Python atlas bake (SAVE_COMPONENT_ATLAS_ORIGIN=(1024,1920),
// 480px cells, SAVE_VIEW_RING_ORIGIN=(314,37), 256x256 panel).
const int kAtlasWidth = 1664;
const int kAtlasHeight = 3840;
const int kAtlasColumnLeft = 1024;
const int kSaveBandTop = 1920;
const int kCellHeight = 480;
const int kRingOriginX = 314;
const int kRingOriginY = 37;
const int kRingSize = 256;

bool NearlyEqual(float actual, float expected)
{
    const float difference =
        actual > expected ? actual - expected : expected - actual;
    // UV ratios (numerator/denominator, both integers, magnitude <= 1) round to
    // within a few ULP.
    return difference <= 1.0e-5f;
}

bool NearlyEqualTexels(float actual, float expected)
{
    const float difference =
        actual > expected ? actual - expected : expected - actual;
    // Texel-space checks multiply a UV difference by the atlas dimension (up to
    // 3840), amplifying float rounding, so allow a small sub-pixel tolerance.
    return difference <= 1.0e-1f;
}
}  // namespace

int main()
{
    // (1) The atlas sample rect must equal the baked panel location for every
    // save selection (0..3).  Left/right span the 256px ring width; top/bottom
    // span its 256px height inside save cell (4 + selection).
    for (fable_u32 selection = 0; selection < 4; ++selection)
    {
        float leftU = -1.0f;
        float topV = -1.0f;
        float rightU = -1.0f;
        float bottomV = -1.0f;
        if (!FableComputeSaveViewportAtlasRect(
                selection,
                kAtlasWidth,
                kAtlasHeight,
                &leftU,
                &topV,
                &rightU,
                &bottomV))
        {
            printf("FABLETLC_SAVE_VIEWPORT_ATLAS_RECT FAIL code=1 sel=%u\n",
                   selection);
            return 1;
        }

        const int cellTop = kSaveBandTop + static_cast<int>(selection) *
                                               kCellHeight;
        const float expectedLeftU =
            static_cast<float>(kAtlasColumnLeft + kRingOriginX) /
            static_cast<float>(kAtlasWidth);
        const float expectedRightU =
            static_cast<float>(kAtlasColumnLeft + kRingOriginX + kRingSize) /
            static_cast<float>(kAtlasWidth);
        const float expectedTopV =
            static_cast<float>(cellTop + kRingOriginY) /
            static_cast<float>(kAtlasHeight);
        const float expectedBottomV =
            static_cast<float>(cellTop + kRingOriginY + kRingSize) /
            static_cast<float>(kAtlasHeight);

        if (!NearlyEqual(leftU, expectedLeftU) ||
            !NearlyEqual(rightU, expectedRightU) ||
            !NearlyEqual(topV, expectedTopV) ||
            !NearlyEqual(bottomV, expectedBottomV))
        {
            printf(
                "FABLETLC_SAVE_VIEWPORT_ATLAS_RECT FAIL code=2 sel=%u "
                "u=[%.6f,%.6f] want[%.6f,%.6f] v=[%.6f,%.6f] "
                "want[%.6f,%.6f]\n",
                selection,
                leftU, rightU, expectedLeftU, expectedRightU,
                topV, bottomV, expectedTopV, expectedBottomV);
            return 2;
        }

        // The sampled rect must be exactly 256px on each axis in atlas texels
        // (a full ring panel, not a clipped or neighbouring-cell rect).
        const float widthTexels =
            (rightU - leftU) * static_cast<float>(kAtlasWidth);
        const float heightTexels =
            (bottomV - topV) * static_cast<float>(kAtlasHeight);
        if (!NearlyEqualTexels(widthTexels, static_cast<float>(kRingSize)) ||
            !NearlyEqualTexels(heightTexels, static_cast<float>(kRingSize)))
        {
            printf(
                "FABLETLC_SAVE_VIEWPORT_ATLAS_RECT FAIL code=3 sel=%u "
                "size=%.3fx%.3f\n",
                selection, widthTexels, heightTexels);
            return 3;
        }

        // Consecutive selections must advance by exactly one 480px cell, so no
        // two selections sample the same rect and none escapes the save band.
        if (selection > 0)
        {
            float prevTopV = 0.0f;
            float prevA = 0.0f;
            float prevB = 0.0f;
            float prevC = 0.0f;
            FableComputeSaveViewportAtlasRect(
                selection - 1, kAtlasWidth, kAtlasHeight,
                &prevA, &prevTopV, &prevB, &prevC);
            const float stepTexels =
                (topV - prevTopV) * static_cast<float>(kAtlasHeight);
            if (!NearlyEqualTexels(stepTexels, static_cast<float>(kCellHeight)))
            {
                printf(
                    "FABLETLC_SAVE_VIEWPORT_ATLAS_RECT FAIL code=4 sel=%u "
                    "step=%.3f\n",
                    selection, stepTexels);
                return 4;
            }
        }
    }

    // (2) Invalid inputs must be rejected (guards against sampling out of the
    // save band or dividing by a zero atlas dimension).
    float unused0 = 0.0f;
    float unused1 = 0.0f;
    float unused2 = 0.0f;
    float unused3 = 0.0f;
    if (FableComputeSaveViewportAtlasRect(
            4, kAtlasWidth, kAtlasHeight,
            &unused0, &unused1, &unused2, &unused3) ||
        FableComputeSaveViewportAtlasRect(
            0, 0, kAtlasHeight,
            &unused0, &unused1, &unused2, &unused3) ||
        FableComputeSaveViewportAtlasRect(
            0, kAtlasWidth, 0,
            &unused0, &unused1, &unused2, &unused3) ||
        FableComputeSaveViewportAtlasRect(
            0, kAtlasWidth, kAtlasHeight,
            0, &unused1, &unused2, &unused3))
    {
        printf("FABLETLC_SAVE_VIEWPORT_ATLAS_RECT FAIL code=5\n");
        return 5;
    }

    printf("FABLETLC_SAVE_VIEWPORT_ATLAS_RECT PASS "
           "selections=4 ring=256x256 atlas=1664x3840\n");
    return 0;
}
