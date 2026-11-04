#include "engine/CCompressorZlib.h"
#include <stdlib.h>
void __cdecl CCompressorZlib::Free(void*,void* p) { free(p); }