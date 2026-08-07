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
// Row order per screen index: 0 = Gameplay (10 rows), 1 = Audio (3 rows,
// entries 3..9 unused = 0), 2 = Video (10 rows).  The frontend manager's
// action order is Gameplay, Video, Audio, Redefine, while the visual detail
// screen ids are Gameplay=1, Audio=2, Video=3.

namespace fable_detail_tables
{
    struct DetailRowDefinition
    {
        const char* label;
        unsigned int valueCount;
        int y;
        unsigned int defaultValue;
    };

    struct DetailScreenDefinition
    {
        unsigned int rowCount;
        DetailRowDefinition rows[10];
    };

    // Decoded CUIDef row records.  The renderer and input bridge consume the
    // same records so a drawn row cannot acquire a different y/value range or
    // default action from its runtime hit-test.
    const DetailScreenDefinition kScreens[3] = {
        {
            10,
            {
                {"Game Camera", 2, 90, 0},
                {"Bow Camera", 2, 120, 0},
                {"Follow Camera", 2, 150, 1},
                {"Camera Sensitivity", 10, 180, 4},
                {"Subtitles", 2, 210, 1},
                {"Interface Opacity", 16, 240, 15},
                {"Tutorials", 2, 270, 1},
                {"Guildmaster Help", 2, 300, 1},
                {"Interface Tooltips", 2, 330, 1},
                {"Show Target Lock", 2, 360, 1}
            }
        },
        {
            3,
            {
                {"Music Volume", 11, 180, 6},
                {"Sound Volume", 11, 210, 8},
                {"Dialogue Volume", 11, 240, 9},
                {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
                {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
                {0, 0, 0, 0}
            }
        },
        {
            10,
            {
                {"Resolution", 3, 90, 1},
                {"Refresh Rate", 4, 120, 0},
                {"Anti-Aliasing", 3, 150, 0},
                {"Texture Detail", 4, 180, 3},
                {"Mesh Detail", 4, 210, 3},
                {"Shadow Detail", 4, 240, 3},
                {"Brightness", 18, 270, 1},
                {"Vertical Sync", 2, 300, 0},
                {"Effects Detail", 4, 330, 3},
                {"Screen Aspect Ratio", 3, 360, 0}
            }
        }
    };

    struct DetailRect
    {
        int left;
        int top;
        int right;
        int bottom;
    };

    struct DetailArrowDefinition
    {
        int controlLeft;
        int controlTopOffset;
        int controlWidth;
        int controlHeight;
        int leftAtlasY;
        int rightAtlasY;
        DetailRect leftHit;
        DetailRect rightHit;
    };

    // UI detail control: the authored row y is three pixels below the 200x30
    // value tile origin; the two arrow hit regions are its 40px end caps.
    const DetailArrowDefinition kArrow = {
        400, -3, 200, 30, 2400, 2430,
        {400, -3, 440, 27},
        {562, -3, 602, 27}
    };

    const int kHoverAtlasOriginX = 1024;

    struct DetailFooterDefinition
    {
        DetailRect hit;
        int atlasY;
        int designX;
        int designY;
        int tileWidth;
        int tileHeight;
    };

    // UI_HELPERS detail footer records.  Atlas Y values are the generated
    // ON-state tiles in the decoded options component sheet.
    const DetailFooterDefinition kFooter[3] = {
        {{20, 424, 276, 480}, 2464, 20, 424, 256, 64},
        {{192, 384, 448, 424}, 2528, 192, 384, 256, 64},
        {{362, 424, 618, 480}, 2592, 362, 424, 256, 64}
    };

    struct RedefineResetDefinition
    {
        DetailRect hit;
        int designX;
        int designY;
        int width;
        int height;
        int atlasY;
    };

    const RedefineResetDefinition kRedefineReset[2] = {
        {{0, 389, 320, 429}, 0, 389, 320, 64, 2720},
        {{320, 389, 640, 429}, 320, 389, 320, 64, 2720}
    };

    struct RedefineListDefinition
    {
        int rootX;
        int rootY;
        int hitOffsetX;
        int hitOffsetY;
        int hitWidth;
        int hitHeight;
        int rowStep;
        int hoverX;
        int hoverY;
        int hoverWidth;
        int hoverHeight;
        int valueX;
        int valueY;
        int valueWidth;
        int valueHeight;
        int helperFrame;
        int helperFrameHeight;
    };

    // UI_FRONTEND_LIST_REDEFINE_KEYS_MENU and its generated mouse-area/value
    // children.  These records bind the runtime hitboxes and helper overlays
    // to the same decoded list geometry.
    const RedefineListDefinition kRedefineList = {
        40, 115, -40, 0, 600, 24, 26,
        8, 110, 588, 35,
        420, 115, 176, 26,
        5, 480
    };

    // Default key values for the Redefine (WASD) scheme.
    // The generated movement children are serialized Forward, Left,
    // Backward, Right.  Their retail WASD profile values are W/A/S/D.
    const unsigned int kRedefineDefaults[9] = {31, 9, 27, 12, 1, 2, 3, 3, 3};

    // Expanded CRedefinerList materialization: the serialized movement action
    // becomes four rows, followed by the remaining ActionOrder children.
    // 0xffffffff means that the retail binding is intentionally unresolved
    // for this bridge; it must remain represented by the static atlas label
    // until a supported input is captured at runtime.
    const unsigned int kRedefineUnresolved = 0xffffffffu;
    const unsigned int kRedefineExpandedDefaults[44] = {
        31, 9, 27, 12, 1, 2, 3, 3, 3,
        4, 5, 6, 35, 55, 36, 36, 23, 15,
        21, 26, 16, 39, 56, 57, 58, 59, 60, 61,
        62, 63, 64, 65, 66, 67, 46, 47, 48, 49,
        50, 51, 52, 53, 54, 68
    };

    // Localized action labels for the expanded CRedefinerList stream.  The
    // movement action is materialized as four rows before the remaining
    // ActionOrder children; these strings are submitted through the live
    // ENG_ARIAL_24 glyph path rather than baked into the component atlas.
    static const char* const kRedefineExpandedLabels[44] = {
        "Move Forward",
        "Move Left",
        "Move Backward",
        "Move Right",
        "Attack",
        "Block",
        "Flourish",
        "Run",
        "Toggle First-Person Targeting",
        "Interact",
        "Unsheathe Melee Weapon",
        "Unsheathe Ranged Weapon",
        "Toggle Target Lock",
        "Toggle Sneak Mode",
        "Suck Experience Orbs",
        "Activate Spell Mode",
        "Cycle Spells (Alternative To Mouse Wheel)",
        "Charge Guild Seal",
        "Toggle Minimap Display",
        "Reset Camera",
        "Toggle View Hero Mode",
        "Toggle In-Game Menu",
        "Use Context Sensitive Item 1",
        "Use Context Sensitive Item 2",
        "Use Context Sensitive Item 3",
        "Shortcut: Items Inventory",
        "Shortcut: Weapons Inventory",
        "Shortcut: Magic Menu",
        "Shortcut: Clothing Inventory",
        "Shortcut: Expressions Menu",
        "Shortcut: Current Quests Screen",
        "Shortcut: Personality Screen",
        "Shortcut: Logbook",
        "Shortcut: Map Screen",
        "Use Hotbar Item 1",
        "Use Hotbar Item 2",
        "Use Hotbar Item 3",
        "Use Hotbar Item 4",
        "Use Hotbar Item 5",
        "Use Hotbar Item 6",
        "Use Hotbar Item 7",
        "Use Hotbar Item 8",
        "Use Hotbar Item 9",
        "Take Photojournal Photo"
    };
}

#endif
