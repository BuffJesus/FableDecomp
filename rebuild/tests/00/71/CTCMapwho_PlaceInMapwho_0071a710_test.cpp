#include <cstdio>

struct C3DVector { float x, y, z; };

struct CTCMapwho {
    unsigned char pad[0x50];
    unsigned char flags;
};

static int g_placed = 0;
static CTCMapwho* g_lastSelf = 0;
static C3DVector* g_lastVec = 0;

struct Slot {
    void Place(CTCMapwho* self, C3DVector* v) {
        g_placed++;
        g_lastSelf = self;
        g_lastVec = v;
    }
};

struct Helper {
    unsigned char pad[0x10];
    Slot* slot;
};

static Slot g_slot;
static Helper g_helper;

Helper* GetMapwhoHelper(void) { g_helper.slot = &g_slot; return &g_helper; }

void PlaceInMapwho(CTCMapwho* self, C3DVector* v)
{
    if (!(self->flags & 1)) {
        Helper* h = GetMapwhoHelper();
        h->slot->Place(self, v);
    }
}

int main()
{
    CTCMapwho m; m.flags = 0;
    C3DVector v = {1,2,3};

    // flag clear -> should place
    PlaceInMapwho(&m, &v);
    if (g_placed != 1 || g_lastSelf != &m || g_lastVec != &v) { printf("FAIL1\n"); return 1; }

    // flag set -> should skip
    m.flags = 1;
    PlaceInMapwho(&m, &v);
    if (g_placed != 1) { printf("FAIL2\n"); return 1; }

    // flag set with other bits, still skip (bit 0 set)
    m.flags = 0x03;
    PlaceInMapwho(&m, &v);
    if (g_placed != 1) { printf("FAIL3\n"); return 1; }

    // flag 0x02 (bit0 clear) -> should place
    m.flags = 0x02;
    PlaceInMapwho(&m, &v);
    if (g_placed != 2) { printf("FAIL4\n"); return 1; }

    printf("PLACEMAPWHO_OK\n");
    return 0;
}