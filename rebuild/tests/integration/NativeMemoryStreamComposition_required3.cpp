#include <stdlib.h>
#include "fable_data_input_stream.h"
extern "C" void UnusedMemoryBoundary(){abort();}
int main(int,char**argv){((CMemoryDataInputStream*)argv)->CMemoryDataInputStream::~CMemoryDataInputStream();return 0;}
#pragma comment(linker,"/include:??1CMemoryDataInputStream@@UAE@XZ")
#pragma comment(linker,"/alternatename:??1CBaseClassNonCopyable@@UAE@XZ=_UnusedMemoryBoundary")
#pragma comment(linker,"/alternatename:??3@YAXPAX@Z=_UnusedMemoryBoundary")
#pragma comment(linker,"/alternatename:??_ECDataStream@@UAEPAXI@Z=_UnusedMemoryBoundary")
