#ifndef FABLE_FRONTEND_LIST_LAYOUT_H
#define FABLE_FRONTEND_LIST_LAYOUT_H

// Authored from the retail frontend.bin UI records and frontend.big sprite
// descriptors. These are serialized list UpArrow/DownArrow bindings, the
// first-state positions of the referenced Type-38 UI components, and the
// exact 32x32 dimensions of FE_SCROLL_*_SPRITE entries 379..382; they are
// not screenshot-derived coordinates.
struct FableFrontendListArrowLayout
{
    unsigned int listDefinitionIndex;
    unsigned int upArrowDefinitionIndex;
    unsigned int downArrowDefinitionIndex;
    int arrowX;
    int upY;
    int downY;
    int arrowWidth;
    int arrowHeight;
    int listX;
    int listY;
    int listHeight;
    int rowStep;
    bool wrapping;
    bool scrolling;
};

// UI_FRONTEND_LIST_FOR_PROFILES (142), UI_PROFILE_LIST_ARROW_UP (426),
// UI_PROFILE_LIST_ARROW_DOWN (423).
static const FableFrontendListArrowLayout
    kFableFrontendProfilesListArrowLayout =
{
    142, 426, 423, 304, 80, 380, 32, 32, 200, 120, 260, 28, false, true
};

// UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE (143),
// UI_DELETE_LIST_ARROW_UP (427), UI_DELETE_LIST_ARROW_DOWN (424).
static const FableFrontendListArrowLayout
    kFableFrontendDeleteListArrowLayout =
{
    143, 427, 424, 304, 140, 380, 32, 32, 200, 180, 210, 28, false, true
};

// UI_FRONTEND_LIST_FOR_SAVES (144), UI_SAVE_LIST_ARROW_UP (428),
// UI_SAVE_LIST_ARROW_DOWN (425).
static const FableFrontendListArrowLayout
    kFableFrontendSaveListArrowLayout =
{
    144, 428, 425, 270, 80, 220, 32, 32, 10, 90, 150, 30, true, true
};

// UI_FRONTEND_LIST_REDEFINE_KEYS_MENU (217),
// UI_REDEFINER_LIST_ARROW_UP (417), UI_REDEFINER_LIST_ARROW_DOWN (420).
static const FableFrontendListArrowLayout
    kFableFrontendRedefineListArrowLayout =
{
    217, 417, 420, 304, 80, 350, 32, 32, 40, 115, 250, 26, false, true
};

#endif
