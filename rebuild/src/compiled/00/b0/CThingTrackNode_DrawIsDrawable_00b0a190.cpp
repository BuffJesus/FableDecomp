struct CThingTrackNode;

struct IDrawableObj {
    // A vtbl of __fastcall member functions. Slot 0x12 (byte offset 0x48)
    // takes one stack arg (the node) with 'this' in ecx.
    int Draw(CThingTrackNode* node);   // placeholder shape; real dispatch via vtbl
};

// helper: __fastcall on the node, returns the drawable object
extern IDrawableObj* __fastcall GetDrawableObject(CThingTrackNode* self);

struct CThingTrackNode {
    IDrawableObj* DrawIsDrawable();
};

// vtable modeled as an array of member-function pointers so the call is __fastcall
// (ecx = obj, one stack arg) with no edx setup.
typedef int (IDrawableObj::*Slot)(CThingTrackNode* node);

IDrawableObj* CThingTrackNode::DrawIsDrawable()
{
    IDrawableObj* obj = GetDrawableObject(this);
    Slot* vtbl = *(Slot**)obj;
    (obj->*vtbl[0x12])(this);
    return obj;
}