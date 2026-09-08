#include "engine/C3DAnimationInfo.h"
#include <cstdio>

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

int main()
{
    C3DAnimationInfo_Methods animation;
    animation.Data_Object = 0;
    if (animation.IsLoading()) return 1;

    CAnimationDataLoadingView data = {};
    animation.Data_Object = (CIVCountedPointeeBase*)&data;
    if (animation.IsLoading()) return 2;

    data.LoadingState = (void*)1;
    if (!animation.IsLoading()) return 3;
    std::printf("ISLOADING_OK\n");
    return 0;
}
