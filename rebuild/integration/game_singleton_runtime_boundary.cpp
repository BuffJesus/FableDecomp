#include "fable_cgame.h"

// The retail CGame singleton (0x013B83D0) and the two unnamed CInitBaseClass
// counters its base constructor and Uninitialise maintain. CGame's own
// Uninitialise (0x00412F60) and the CInitBaseClass bodies are linked from
// their verified leaves; nothing here re-implements them.
long g_CInitBaseClassConstructed_013BCA18 = 0;
long g_CInitBaseClassValid_013BCA14 = 0;

CGame g_Game_013B83D0;
