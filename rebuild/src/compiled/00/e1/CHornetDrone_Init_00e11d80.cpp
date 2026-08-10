#include <cstdlib>

struct CHornetDrone {
    char pad[0x1c];
    int field_1c;
};

void __fastcall Init(CHornetDrone* self)
{
    self->field_1c = (rand() % 200) + 1;
}