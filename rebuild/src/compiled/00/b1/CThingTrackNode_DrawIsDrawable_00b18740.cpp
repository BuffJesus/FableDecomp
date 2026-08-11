// CThingTrackNode::DrawIsDrawable @ 0x00b18740
// retail: 56 57 8bf9 e8.. 8bf0 8b06 57 8bce ff5048 5f 8bc6 5e c3
// same shape as 0x00afe580.

struct CThingTrackNode;

struct IDrawableObj {
    int Draw(CThingTrackNode* node);   // placeholder shape; real dispatch via vtbl
};

// helper @ 0x00af7fb0 : __fastcall on the node, returns the drawable object
extern IDrawableObj* __fastcall CreateInternalPrimitive(CThingTrackNode* self);

struct CThingTrackNode {
    IDrawableObj* DrawIsDrawable();
};

// vtable modeled as an array of member-function pointers so the call is __fastcall
// (ecx = obj, one stack arg) with no edx setup.
typedef int (IDrawableObj::*Slot)(CThingTrackNode* node);

IDrawableObj* CThingTrackNode::DrawIsDrawable()
{
    IDrawableObj* obj = CreateInternalPrimitive(this);
    Slot* vtbl = *(Slot**)obj;
    (obj->*vtbl[0x12])(this);
    return obj;
}