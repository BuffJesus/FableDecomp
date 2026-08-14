#include <cstdio>

struct C2DVector { float x; float y; };

struct SubObj {
    int first;
    int result_to_return;
    int GetSomething(C2DVector *v) { (void)v; return result_to_return; }
};

struct CNavQuadTree {
    char pad[0x10];
    SubObj sub;
};

static bool IsActionPoint(CNavQuadTree *self, C2DVector *v)
{
    SubObj *s = &self->sub;
    int r = s->GetSomething(v);
    return r != s->first;
}

int main()
{
    CNavQuadTree t;
    C2DVector v = {1.0f, 2.0f};

    t.sub.first = 5;
    t.sub.result_to_return = 9;
    bool a = IsActionPoint(&t, &v);

    t.sub.first = 7;
    t.sub.result_to_return = 7;
    bool b = IsActionPoint(&t, &v);

    if (a == true && b == false) {
        printf("BEHAVIOR_OK\n");
    } else {
        printf("BEHAVIOR_FAIL a=%d b=%d\n", (int)a, (int)b);
    }
    return 0;
}