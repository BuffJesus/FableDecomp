#include "engine/C3DAnimationInfo.h"  // retyped onto the PDB layout; byte parity re-verified

// Retail's counted pointee has a loader-state pointer at +0x30. The donor header
// intentionally leaves that pointee opaque, so retain only this retail view.
struct CAnimationDataLoadingView {
    unsigned char _pad_0x00[0x30];
    void* LoadingState;
};

struct C3DAnimationInfo_Methods : C3DAnimationInfo {
    bool IsLoading();
};

bool C3DAnimationInfo_Methods::IsLoading()
{
    CAnimationDataLoadingView* data =
        (CAnimationDataLoadingView*)Data_Object;
    if (data && data->LoadingState) return true;
    return false;
}
