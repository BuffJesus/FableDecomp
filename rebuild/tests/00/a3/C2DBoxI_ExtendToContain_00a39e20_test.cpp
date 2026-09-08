#include <stdio.h>
#include "engine/C2DBoxI.h"
int main() { C2DBoxI box = { 10, 20, 30, 40 }; C2DCoordI point = { 5, 50 }; box.ExtendToContain(point); if (box.TLX != 5 || box.TLY != 20 || box.BRX != 30 || box.BRY != 50) return 1; point.X = 25; point.Y = 30; box.ExtendToContain(point); if (box.TLX != 5 || box.TLY != 20 || box.BRX != 30 || box.BRY != 50) return 2; printf("EXTEND_BOX_TO_CONTAIN PASS\n"); return 0; }
