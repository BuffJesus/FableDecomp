#pragma optimize("g",on)
struct CThingTrackNode;

// Object with a virtual method occupying vtable slot 0x12 (offset 0x48).
struct IDrawableBase {
    virtual int pad00();
    virtual int pad01();
    virtual int pad02();
    virtual int pad03();
    virtual int pad04();
    virtual int pad05();
    virtual int pad06();
    virtual int pad07();
    virtual int pad08();
    virtual int pad09();
    virtual int pad10();
    virtual int pad11();
    virtual int pad12();
    virtual int pad13();
    virtual int pad14();
    virtual int pad15();
    virtual int pad16();
    virtual int pad17();
    virtual int slot18(CThingTrackNode* node); // offset 0x48
};

extern IDrawableBase* __fastcall GetDrawableObject(CThingTrackNode* self);

struct CThingTrackNode {
    IDrawableBase* DrawIsDrawable();
};

IDrawableBase* CThingTrackNode::DrawIsDrawable()
{
    IDrawableBase* obj = GetDrawableObject(this);
    obj->slot18(this);
    return obj;
}