#include <cstdio>

struct FadeEntry { float min_; float scale; };
struct Gen { char pad[0x3c]; FadeEntry entries[64]; };

static float Modify(Gen* self, unsigned long index, float value)
{
    float product = value * self->entries[index].scale;
    if (product > self->entries[index].min_)
        return product;
    return self->entries[index].min_;
}

int main()
{
    Gen g;
    for (int i = 0; i < 64; i++) { g.entries[i].min_ = 0.0f; g.entries[i].scale = 0.0f; }
    g.entries[2].min_ = 10.0f; g.entries[2].scale = 2.0f;

    int ok = 1;
    if (Modify(&g, 2, 100.0f) != 200.0f) ok = 0;
    if (Modify(&g, 2, 1.0f) != 10.0f) ok = 0;
    if (Modify(&g, 2, 5.0f) != 10.0f) ok = 0;

    if (ok) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL\n");
    return 0;
}