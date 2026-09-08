#include "engine/CDisplayViewManager.h"

struct CInterpolationInfo;
struct CViewBase {
    virtual void Draw(CInterpolationInfo* info, bool b) = 0;
};

struct CDisplayViewManager_Methods : CDisplayViewManager {
    void Render(CInterpolationInfo* info, bool b);
};

extern CViewBase* __fastcall GetFallbackView_c31a70(CDisplayViewManager* self);

void CDisplayViewManager_Methods::Render(CInterpolationInfo* info, bool b)
{
    CViewBase* v = this->PCurrentView_Data;
    if (v)
        v->Draw(info, b);
    else
        GetFallbackView_c31a70(this)->Draw(info, b);
}
