#include <cstdio>

// Behaviour model: GetTotalTimeSeconds converts an internal tick count
// (from a virtual) into seconds via a helper fetched from a subobject.

struct Helper { float rate; };

static Helper g_helper = { 0.001f };  // 1 tick = 0.001 s

static Helper* GetHelperFromSubB(void* subb) { return &g_helper; }
static float   HelperConvert(Helper* h, int arg) { return h->rate * (float)arg; }

struct Model {
    int tickValue;
    virtual int v7() { return tickValue; }
    float GetTotalTimeSeconds() {
        Helper* h = GetHelperFromSubB((char*)this + 8);
        int ticks = this->v7();
        return HelperConvert(h, ticks);
    }
};

int main() {
    Model m; m.tickValue = 5000;
    float s = m.GetTotalTimeSeconds();
    // 5000 * 0.001 = 5.0
    if (s > 4.999f && s < 5.001f) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL %f\n", s);
    }
    return 0;
}