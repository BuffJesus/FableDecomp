#include <stdlib.h>
#include "fable_graphic_frame_data.h"
#pragma comment(linker,"/include:?Uninitialise@CGraphicFrameData@@QAEXXZ")
extern "C" void UnusedCompositionBoundary(){abort();}
int main(int,char**argv){((CGraphicFrameData*)argv)->Uninitialise();return 0;}
#pragma comment(linker,"/alternatename:??3@YAXPAX@Z=_UnusedCompositionBoundary")
