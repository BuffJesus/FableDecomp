#include "fable_volume_types.h"
void CVolumeTexture::PreLoad() { PD3DVolumeTexture->PreLoad(); }
bool CVolumeTexture::IsBusy() const { return false; }
bool CVolumeTexture::Compress() { return false; }
void CVolumeTexture::GenerateMipmaps(unsigned long, unsigned long) {}
