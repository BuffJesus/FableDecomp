#include <stdlib.h>
#pragma comment(linker,"/include:?InitialiseFrame@CGraphicInfoFrame@@AAEXAAVCGraphicFrame@@AAVCManagedTexture@@C@Z")
extern "C" void __fastcall RequiredCompositionProvider(void*,void*,void*,void*,signed char);
extern "C" void UnusedCompositionBoundary(){abort();}
int main(){RequiredCompositionProvider(0,0,0,0,0);return 0;}
#pragma comment(linker,"/alternatename:@RequiredCompositionProvider@20=?InitialiseFrame@CGraphicInfoFrame@@AAEXAAVCGraphicFrame@@AAVCManagedTexture@@C@Z")
#pragma comment(linker,"/alternatename:??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z=_UnusedCompositionBoundary")
#pragma comment(linker,"/alternatename:??2@YAPAXI@Z=_UnusedCompositionBoundary")
#pragma comment(linker,"/alternatename:??3@YAXPAX@Z=_UnusedCompositionBoundary")
#pragma comment(linker,"/alternatename:??4CManagedTexture@@QAEABV0@ABV0@@Z=_UnusedCompositionBoundary")
#pragma comment(linker,"/alternatename:?Release@CManagedTexture@@QAE_NXZ=_UnusedCompositionBoundary")
#pragma comment(linker,"/alternatename:?Uninitialise@CGraphicFrameData@@QAEXXZ=_UnusedCompositionBoundary")
