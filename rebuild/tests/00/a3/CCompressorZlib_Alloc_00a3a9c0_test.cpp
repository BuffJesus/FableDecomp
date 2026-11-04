#include "test_support/compressor_test_access.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
int main(){
 if(CCompressorZlibTestAccess::Alloc(0,0,9) || CCompressorZlibTestAccess::Alloc(0,5,0) || CCompressorZlibTestAccess::Alloc(0,0xffffffffU,0xffffffffU) || CCompressorZlibTestAccess::Alloc(0,65536,65536) || CCompressorZlibTestAccess::Alloc(0,0x80000000U,1))return 1;
 void* p=CCompressorZlibTestAccess::Alloc((void*)1,128,256);if(!p || _msize(p)<32768)return 2;free(p);
 puts("ALLOC_PASS");return 0;
}
