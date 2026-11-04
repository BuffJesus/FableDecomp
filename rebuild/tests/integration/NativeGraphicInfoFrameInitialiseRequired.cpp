#include <stdlib.h>
extern "C" void __fastcall RequiredFrameInitialise(void*,void*,void*,void*,signed char);
extern "C" void UnusedBoundary(){abort();}
int main(){RequiredFrameInitialise(0,0,0,0,0);return 0;}
#pragma comment(linker,"/alternatename:@RequiredFrameInitialise@20=?InitialiseFrame@CGraphicInfoFrame@@AAEXAAVCGraphicFrame@@AAVCManagedTexture@@C@Z")
#pragma comment(linker,"/alternatename:??2@YAPAXI@Z=_UnusedBoundary")
#pragma comment(linker,"/alternatename:??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z=_UnusedBoundary")
#pragma comment(linker,"/alternatename:?Uninitialise@CGraphicFrameData@@QAEXXZ=_UnusedBoundary")
#pragma comment(linker,"/alternatename:??3@YAXPAX@Z=_UnusedBoundary")
#pragma comment(linker,"/alternatename:??4CManagedTexture@@QAEABV0@ABV0@@Z=_UnusedBoundary")
#pragma comment(linker,"/alternatename:?Release@CManagedTexture@@QAE_NXZ=_UnusedBoundary")
