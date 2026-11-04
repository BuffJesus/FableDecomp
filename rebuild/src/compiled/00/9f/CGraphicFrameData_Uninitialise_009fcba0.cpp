#include "fable_graphic_frame_data.h"
template<class T> inline void CCountedPointer<T>::ReleaseData() {
    Release();
    Info=0;
    Data=0;
}
void CGraphicFrameData::Uninitialise() {
    IsInitialised=0;
    if(UsesSurface)PTexture.ReleaseData();
}
