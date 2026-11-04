#include "test_support/compressor_test_access.h"
#include <stdio.h>
static int calls;static void* released;
extern "C" void __cdecl free(void* p){++calls;released=p;}
int main(){calls=0;CCompressorZlibTestAccess::Free((void*)99,(void*)123);if(calls!=1 || released!=(void*)123)return 1;
 CCompressorZlibTestAccess::Free((void*)99,0);if(calls!=2 || released)return 2;puts("FREE_PASS");return 0;}
