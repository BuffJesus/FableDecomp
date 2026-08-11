#pragma optimize("g",on)
struct CThingTrackNode;

// Non-polymorphic class: member function pointer is a single 4-byte code pointer.
struct IDrawableObj {
    void* vtbl;
    int Slot12(CThingTrackNode* node);
};

typedef int (IDrawableObj::*DrawPMF)(CThingTrackNode*);

extern IDrawableObj* __fastcall GetDrawableObject(CThingTrackNode* self);

struct CThingTrackNode {
    IDrawableObj* DrawIsDrawable();
};

IDrawableObj* CThingTrackNode::DrawIsDrawable()
{
    IDrawableObj* obj = GetDrawableObject(this);
    void** vt = (void**)obj->vtbl;
    void* raw = vt[0x12];
    DrawPMF pmf;
    *(void**)&pmf = raw;
    (obj->*pmf)(this);
    return obj;
}