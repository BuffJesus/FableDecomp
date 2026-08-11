#pragma optimize("g",on)
struct CThingTrackNode;

struct IDrawableObj {
    // slot 0x12 (offset 0x48): __fastcall method taking a CThingTrackNode*
    virtual void pad00();
    virtual void pad01();
    virtual void pad02();
    virtual void pad03();
    virtual void pad04();
    virtual void pad05();
    virtual void pad06();
    virtual void pad07();
    virtual void pad08();
    virtual void pad09();
    virtual void pad10();
    virtual void pad11();
    virtual void pad12();
    virtual void pad13();
    virtual void pad14();
    virtual void pad15();
    virtual void pad16();
    virtual void pad17();
    virtual int slot12(CThingTrackNode* node);
};

extern IDrawableObj* __fastcall GetDrawableObject(CThingTrackNode* self);

struct CThingTrackNode {
    IDrawableObj* DrawIsDrawable();
};

IDrawableObj* CThingTrackNode::DrawIsDrawable()
{
    IDrawableObj* obj = GetDrawableObject(this);
    obj->slot12(this);
    return obj;
}