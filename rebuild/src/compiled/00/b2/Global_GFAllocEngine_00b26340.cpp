#include <new>
#include "fable_engine.h"
#include "fable_engine_init.h"
CIEngine* FABLE_FASTCALL GFAllocEngine() { return new CEngine; }
