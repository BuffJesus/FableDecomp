#include <stdio.h>

struct Filter {
    unsigned char enabled;
    float amount;
    int rendered;
};

const float g_threshold = 0.0f;

static void DoRender(Filter* self) { self->rendered++; }

static void Render(Filter* self) {
    if (self->enabled) {
        if (self->amount > g_threshold) {
            DoRender(self);
        }
    }
}

int main() {
    int ok = 1;
    // disabled: never renders
    { Filter f = {0, 5.0f, 0}; Render(&f); if (f.rendered != 0) ok = 0; }
    // enabled, amount > threshold: renders
    { Filter f = {1, 5.0f, 0}; Render(&f); if (f.rendered != 1) ok = 0; }
    // enabled, amount == threshold: NOT greater -> no render
    { Filter f = {1, 0.0f, 0}; Render(&f); if (f.rendered != 0) ok = 0; }
    // enabled, amount < threshold: no render
    { Filter f = {1, -1.0f, 0}; Render(&f); if (f.rendered != 0) ok = 0; }
    if (ok) printf("BEHAVIOUR_OK\n");
    else printf("BEHAVIOUR_FAIL\n");
    return 0;
}