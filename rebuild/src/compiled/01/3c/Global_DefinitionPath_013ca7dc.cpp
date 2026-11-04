#include "fable_string.h"

// CRT initializer 0x01224050 constructs this, and its paired exit thunk
// 0x0122A6B0 destroys it. GFMain writes the compiled-definition directory.
CWideString g_013ca7dc;
