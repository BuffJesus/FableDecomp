#include "fable_stopwatch.h"

// Force the real inline member's out-of-line COMDAT to be emitted, as retail
// does. This anchor is link scaffolding; the method body is in the header.
typedef __int64 (CStopWatch::*StopWatchTicksMethod)() const;
extern const StopWatchTicksMethod g_FableStopWatchGetTicks_0062F790 = &CStopWatch::GetTicks;
