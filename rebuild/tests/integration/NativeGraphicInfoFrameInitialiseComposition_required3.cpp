#include <stdlib.h>
#include "fable_managed_texture.h"
#pragma comment(linker,"/include:?Release@CManagedTexture@@QAE_NXZ")
extern "C" void UnusedCompositionBoundary(){abort();}
int main(int,char**argv){return ((CManagedTexture*)argv)->Release();}
#pragma comment(linker,"/alternatename:?ReleaseTexture@CManagedTextureData@@QAEXXZ=_UnusedCompositionBoundary")
