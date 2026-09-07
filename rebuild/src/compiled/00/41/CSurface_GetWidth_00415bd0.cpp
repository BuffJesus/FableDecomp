// Getter that materialises a temporary, fills it through an out-param member
// call, and returns one word of it. __fastcall this=ecx, no args.
#include "engine/CSurface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Rect { int width; int height; };
struct CSurface_Methods : CSurface {
    Rect* GetRect(Rect* out);
    int Get();
};
int CSurface_Methods::Get() {
    Rect r;
    return this->GetRect(&r)->width;
}