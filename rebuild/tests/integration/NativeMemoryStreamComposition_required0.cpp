#include <stdlib.h>
#include "fable_data_input_stream.h"
extern "C" void UnusedMemoryBoundary(){abort();}
int main(int,char**argv){((CMemoryDataInputStream*)argv)->CMemoryDataInputStream::ReadCompressed(argv+1,4);return 0;}
#pragma comment(linker,"/include:?ReadCompressed@CMemoryDataInputStream@@UAEXPAXJ@Z")
#pragma comment(linker,"/alternatename:?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z=_UnusedMemoryBoundary")
#pragma comment(linker,"/alternatename:?m_errorCallback@CAFile@@2P6IXXZA=_UnusedMemoryBoundary")
#pragma comment(linker,"/alternatename:_lzo1x_decompress_asm_fast=_UnusedMemoryBoundary")
