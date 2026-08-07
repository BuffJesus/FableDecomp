#include "rebuild_abi.h"
struct G { int Helper(int a); };
extern G* g_beb540;
extern "C" int FABLE_FASTCALL New_beb540(int arg){ return g_beb540->Helper(arg); }