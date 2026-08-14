struct C2DVector { float x; float y; };

struct SubObj {
    int first;
    int GetSomething(C2DVector *v);
};

struct CNavQuadTree {
    char pad[0x10];
    SubObj sub;
};

bool __fastcall IsActionPoint(CNavQuadTree *self, void *edx, C2DVector *v)
{
    (void)edx;
    SubObj *s = &self->sub;
    int r = s->GetSomething(v);
    return r != s->first;
}