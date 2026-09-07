#include "engine/CThingGameObject.h"
#include <stdio.h>


void model(CThingGameObject *self, bool value)
{
    self->flags = (unsigned char)((((value & 1) | 2) << 2) | (self->flags & 0xfb));
}

int main()
{
    CThingGameObject o;
    o.flags = 0xFF;
    model(&o, true);
    if ((o.flags & 0x04) == 0) { printf("FAIL1\n"); return 1; }

    o.flags = 0x00;
    model(&o, true);
    if (o.flags != 0x0C) { printf("FAIL2 %02x\n", o.flags); return 1; }

    o.flags = 0x00;
    model(&o, false);
    if (o.flags != 0x08) { printf("FAIL3 %02x\n", o.flags); return 1; }

    o.flags = 0xF3;
    model(&o, true);
    if (o.flags != 0xFF) { printf("FAIL4 %02x\n", o.flags); return 1; }

    o.flags = 0xFF;
    model(&o, false);
    if (o.flags != 0xFB) { printf("FAIL5 %02x\n", o.flags); return 1; }

    printf("BEHAVIOUR_OK\n");
    return 0;
}