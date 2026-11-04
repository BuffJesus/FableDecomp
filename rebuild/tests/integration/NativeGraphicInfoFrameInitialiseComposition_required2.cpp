#include <stdlib.h>
#include "fable_managed_texture.h"
#pragma comment(linker,"/include:??4CManagedTexture@@QAEABV0@ABV0@@Z")
extern "C" void UnusedCompositionBoundary(){abort();}
int main(int,char**argv){*((CManagedTexture*)argv)=*((CManagedTexture*)(argv+1));return 0;}
#pragma comment(linker,"/alternatename:?Release@CManagedTexture@@QAE_NXZ=_UnusedCompositionBoundary")
