#include <stdlib.h>
#include "fable_data_input_stream.h"
extern "C" void UnusedMemoryBoundary(){abort();}
int main(int,char**argv){((CDataInputStream*)argv)->ReadWithSrcChunkOverflow(argv+1,4);return 0;}
#pragma comment(linker,"/include:?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z")
