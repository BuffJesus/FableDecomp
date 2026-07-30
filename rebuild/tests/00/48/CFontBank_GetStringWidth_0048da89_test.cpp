
#include <stdio.h>

struct Vec2 { float x; float y; };

struct CFontBank {
    virtual Vec2* slot0(Vec2* out, const char* s, int f) = 0;
    virtual Vec2* Measure(Vec2* out, const char* s, int f) = 0;
    float GetStringWidth(const char* s);
};

float CFontBank::GetStringWidth(const char* s) {
    Vec2 tmp;
    Vec2* r = this->Measure(&tmp, s, 0);
    return r->x;
}

// concrete impl to exercise the virtual dispatch
struct FakeFont : public CFontBank {
    Vec2 store;
    Vec2* slot0(Vec2* out, const char* s, int f) { out->x = -1.0f; out->y = -1.0f; return out; }
    Vec2* Measure(Vec2* out, const char* s, int f) {
        // width = 7 * strlen, flag must be 0
        int n = 0; const char* p = s; while (*p) { n++; p++; }
        out->x = 7.0f * (float)n;
        out->y = (float)f; // must be 0
        store = *out;
        return &store;
    }
};

int main() {
    FakeFont ff;
    float w = ff.GetStringWidth("HELLO"); // 5 chars -> 35.0
    if (w == 35.0f) {
        printf("OK_0x0048da89 w=%f\n", w);
        return 0;
    }
    printf("BAD w=%f\n", w);
    return 1;
}