#include <cstdio>

struct C3DVector { float x, y, z; };

struct Inner {
    virtual bool Check(float f, C3DVector* v) {
        // model: reset navigation if destination differs beyond threshold f
        return (v->x + v->y + v->z) > f;
    }
};

struct CTCCreatureNavigation {
    char pad[0x50];
    Inner* nav;
};

bool __fastcall IsNewDestinationGoingToResetNavigation_v2(CTCCreatureNavigation* self, int /*edx*/, C3DVector* v, float f) {
    return self->nav->Check(f, v);
}

int main() {
    Inner inner;
    CTCCreatureNavigation obj;
    obj.nav = &inner;

    C3DVector v1 = {10.0f, 0.0f, 0.0f};
    bool r1 = IsNewDestinationGoingToResetNavigation_v2(&obj, 0, &v1, 5.0f); // 10 > 5 -> true
    C3DVector v2 = {1.0f, 0.0f, 0.0f};
    bool r2 = IsNewDestinationGoingToResetNavigation_v2(&obj, 0, &v2, 5.0f); // 1 > 5 -> false

    if (r1 && !r2) {
        printf("OK_00c43370\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}