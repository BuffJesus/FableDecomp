// CThingTrackNode::DrawIsDrawable @ 00b05ab0
struct CThingTrackNode;

struct Obj {
    // vtable: slot index 18 (byte offset 0x48) is the called virtual
    virtual void s00()=0; virtual void s01()=0; virtual void s02()=0; virtual void s03()=0;
    virtual void s04()=0; virtual void s05()=0; virtual void s06()=0; virtual void s07()=0;
    virtual void s08()=0; virtual void s09()=0; virtual void s10()=0; virtual void s11()=0;
    virtual void s12()=0; virtual void s13()=0; virtual void s14()=0; virtual void s15()=0;
    virtual void s16()=0; virtual void s17()=0;
    virtual void s18(CThingTrackNode* arg)=0; // [vtbl+0x48]
};

extern "C" Obj* __fastcall CThingTrackNode_GetObj(CThingTrackNode* self); // 0xaf7d80

Obj* __fastcall DrawIsDrawable(CThingTrackNode* self)
{
    Obj* t = CThingTrackNode_GetObj(self);
    t->s18(self);
    return t;
}