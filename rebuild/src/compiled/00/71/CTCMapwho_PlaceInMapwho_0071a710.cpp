// Byte-exact reconstruction of CTCMapwho::PlaceInMapwho @ 0x0071a710

struct C3DVector { float x, y, z; };

struct Helper;
struct Slot;

struct CTCMapwho {
    char pad[0x50];
    unsigned char flags;       // +0x50
};

// global returning Helper* (0x686d30) -- no args, cdecl
extern "C" Helper* __cdecl GetMapwhoHelper(void);

// The object stored at Helper+0x10. Retail method 0x637fc0 is a native __fastcall:
// this in ecx (= Helper+0x10), and two stack args (mapwho, vector). Modeled as a
// real C++ member so VC7.1 emits a native thiscall (this in ecx, args on stack) --
// no __fastcall keyword, so the harness cannot rewrite it and no edx is touched.
struct Slot {
    void Place(CTCMapwho* mw, C3DVector* v);
};

// Helper layout: pointer to Slot at +0x10
struct Helper {
    char pad[0x10];
    Slot* slot;                // +0x10
};

// Retail PlaceInMapwho is __fastcall: this in ecx, the C3DVector* on the stack.
// The harness rewrites __fastcall->__fastcall, so model the this pointer in
// ecx and force the vector onto the stack by consuming edx with a dummy arg.
void __fastcall PlaceInMapwho(CTCMapwho* self, int /*edx*/, C3DVector* v)
{
    if (!(self->flags & 1)) {
        Helper* h = GetMapwhoHelper();
        h->slot->Place(self, v);
    }
}