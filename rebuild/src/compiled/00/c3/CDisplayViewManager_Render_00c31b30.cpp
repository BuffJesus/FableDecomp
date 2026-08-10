struct CInterpolationInfo;

// Object with a vtable whose slot 0 takes (CInterpolationInfo*, bool) as __fastcall.
struct IRenderable {
    virtual void Draw(CInterpolationInfo* info, bool b) = 0;   // vtbl slot 0
};

struct CDisplayViewManager {
    IRenderable* pView;   // +0
    void Render(CInterpolationInfo* info, bool b);
};

extern IRenderable* __fastcall GetFallbackView_c31a70(CDisplayViewManager* self);

void CDisplayViewManager::Render(CInterpolationInfo* info, bool b)
{
    IRenderable* v = this->pView;
    if (v)
        v->Draw(info, b);
    else
        GetFallbackView_c31a70(this)->Draw(info, b);
}