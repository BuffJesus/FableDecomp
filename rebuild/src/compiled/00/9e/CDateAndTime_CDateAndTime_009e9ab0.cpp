// CDateAndTime::CDateAndTime @ 0x009E9AB0 (FableWin ??0CDateAndTime@@QAE@XZ).
// A zeroed date: every field 0.

#include "fable_game.h"

CDateAndTime::CDateAndTime()
    : Year(0),
      Month(0),
      Day(0),
      Hour(0),
      Minute(0),
      Second(0),
      Millisecond(0)
{
}
