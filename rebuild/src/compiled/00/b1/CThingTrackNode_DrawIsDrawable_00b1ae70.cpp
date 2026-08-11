struct CThingTrackNode;

struct CPrim
{
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void Draw(CThingTrackNode* node);
};

CPrim* __fastcall CreatePrim(CThingTrackNode* self);

struct CThingTrackNode
{
    CPrim* __fastcall DrawIsDrawable();
};

CPrim* __fastcall CThingTrackNode::DrawIsDrawable()
{
    CPrim* p = CreatePrim(this);
    p->Draw(this);
    return p;
}