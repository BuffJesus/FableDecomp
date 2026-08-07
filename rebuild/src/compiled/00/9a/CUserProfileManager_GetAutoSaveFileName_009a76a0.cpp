#include "rebuild_abi.h"
extern char g_str_9a76a0;
struct C { int x; void Helper(char* s); C* Get(); };
C* C::Get(){ this->Helper(&g_str_9a76a0); return this; }