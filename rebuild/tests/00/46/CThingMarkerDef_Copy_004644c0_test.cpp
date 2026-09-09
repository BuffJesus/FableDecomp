#include <stdio.h>
#include <string.h>
#include "engine/CThingMarkerDefRetail.h"
void CThingBaseDefRetail::CopyBase(const CDefClassBase*) {}
int main() { CThingMarkerDef source, target; memset(&source, 0, sizeof(source)); memset(&target, 0x5a, sizeof(target)); for (int i = 0; i < 14; ++i) source.Graphic.bytes[i] = (unsigned char)(i + 1); source.TriggerType = 4; source.Properties = 0x12345678; target.CThingMarkerDef::Copy(&source); if (memcmp(target.Graphic.bytes, source.Graphic.bytes, 14) || target._graphicTail[0] != 0x5a || target.TriggerType != 4 || target.Properties != 0x12345678) return 1; printf("THING_MARKER_COPY PASS\n"); return 0; }