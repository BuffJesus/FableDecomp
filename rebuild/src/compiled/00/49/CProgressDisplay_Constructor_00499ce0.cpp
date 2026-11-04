#include "fable_boot.h"

CProgressDisplay::CProgressDisplay()
    : vftable00(&g_CProgressDisplayVTable_012388B4),
      LoadingScreenIndex(0),
      value44(0),
      value54(0),
      value58(0),
      progressTime60(0.0),
      displayTime68(0.0),
      completionTime70(0.0),
      flag78(0),
      active79(false),
      drawRetail7A(1),
      flag7B(0),
      displayText7C(1),
      flag7D(0),
      flag7E(0),
      flag7F(0),
      flag80(0),
      flag81(0)
{
}
