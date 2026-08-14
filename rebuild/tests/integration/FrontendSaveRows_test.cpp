/* FrontendSaveRows_test.cpp — offline unit test for the live save-row feeder.
 *
 * Runs FableBuildSaveRowsForTest against a real profile directory and prints the
 * assembled rows as machine-readable lines. A python driver
 * (tools/tests/run_frontend_save_rows.py) compares these against the
 * save_metadata.py golden for every local profile. Also exercises the feeder
 * entry point through a recording stub of the renderer sink.
 *
 * Usage: FrontendSaveRows_test.exe <profileDir>
 */
#include "frontend_save_rows.h"
#include "fable_visual_d3d9.h"

#include <stdio.h>
#include <wchar.h>
#include <string.h>

/* recording stub of the renderer sink (real impl lives in visual_boot_d3d9.cpp).
 * Plain C++ linkage to match the fable_visual_d3d9.h declaration. */
static int        g_sinkCount = -1;
static char       g_sinkNames[4][128];
static fable_u32  g_sinkActions[4];

void FABLE_FASTCALL FableSetVisualFrontendSaveRows(
    const char* const* names, const fable_u32* actions, fable_u32 count)
{
    unsigned int i;
    if (names == 0) { g_sinkCount = 0; return; }   /* reset-to-defaults */
    g_sinkCount = (int)count;
    for (i = 0; i < count && i < 4; ++i) {
        strncpy(g_sinkNames[i], names[i], 127);
        g_sinkNames[i][127] = 0;
        g_sinkActions[i] = actions[i];
    }
}

int wmain(int argc, wchar_t** argv)
{
    FableSaveRowOut rows[4];
    unsigned int n, i;
    if (argc < 2) { printf("usage: FrontendSaveRows_test <profileDir>\n"); return 2; }

    n = FableBuildSaveRowsForTest(argv[1], rows, 4);
    for (i = 0; i < n; ++i)
        printf("ROW\t%s\t0x%02x\n", rows[i].name, (unsigned)rows[i].action);
    printf("COUNT\t%u\n", n);

    /* also drive the production entry point through the recording stub */
    FableFeedVisualFrontendSaveRows(argv[1]);
    if (g_sinkCount != (int)n) {
        printf("SINK_MISMATCH\tfeeder=%d build=%u\n", g_sinkCount, n);
        return 1;
    }
    for (i = 0; i < n; ++i) {
        if (strcmp(g_sinkNames[i], rows[i].name) != 0 ||
            g_sinkActions[i] != rows[i].action) {
            printf("SINK_ROW_MISMATCH\t%u\n", i);
            return 1;
        }
    }
    printf("SINK_OK\t%d\n", g_sinkCount);
    return 0;
}
