#ifndef FABLE_DETAIL_SCREEN_TABLES_H
#define FABLE_DETAIL_SCREEN_TABLES_H

// Single source of truth for the detail option-screen (Gameplay/Video/Audio)
// geometry, shared by the INPUT hit-test (visual_boot_checkpoint.cpp) and the
// RENDER path (visual_boot_d3d9.cpp).  Previously each file hand-duplicated
// these four tables under different names (kVisualDetail* vs kDetail*); they
// were identical, but any future edit to one and not the other would silently
// misalign a clickable region from its drawn control (audit #4).  Both files
// now bind their local names to these definitions via references, so the values
// cannot drift.  const namespace-scope arrays have internal linkage, so each
// translation unit gets its own copy with no ODR/link conflict.
//
// Row order per screen index: 0 = Gameplay (10 rows), 1 = Video (3 rows,
// entries 3..9 unused = 0), 2 = Audio (10 rows).

namespace fable_detail_tables
{
    const unsigned int kRowCounts[3] = {10, 3, 10};

    const unsigned int kValueCounts[3][10] = {
        {2, 2, 2, 10, 2, 16, 2, 2, 2, 2},
        {11, 11, 11, 0, 0, 0, 0, 0, 0, 0},
        {3, 4, 3, 4, 4, 4, 18, 2, 4, 3}
    };

    const int kRowY[3][10] = {
        {90, 120, 150, 180, 210, 240, 270, 300, 330, 360},
        {130, 190, 250, 0, 0, 0, 0, 0, 0, 0},
        {90, 120, 150, 180, 210, 240, 270, 300, 330, 360}
    };

    // Default key values for the Redefine (WASD) scheme.
    const unsigned int kRedefineDefaults[9] = {31, 27, 9, 12, 1, 2, 3, 3, 3};
}

#endif
