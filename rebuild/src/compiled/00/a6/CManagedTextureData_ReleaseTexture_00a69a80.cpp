#include "fable_managed_texture_data.h"
template<class T> inline void CCountedPointer<T>::ReleaseData() {
    Release();
    Info=0;
    Data=0;
}
void CManagedTextureData::ReleaseTexture() {
    Allocated=false;
    Owner=0;
    VolumeTexture.ReleaseData();
    IDirect3DVolumeTexture9* header=D3DVolumeHeader_ptr;
    D3DVolumeHeader_ptr=0;
    operator delete(header);
    SetAsLRU();
}
