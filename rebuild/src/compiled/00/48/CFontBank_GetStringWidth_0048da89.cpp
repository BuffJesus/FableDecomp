#pragma optimize("s",on)

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