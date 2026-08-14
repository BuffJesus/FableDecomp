#include <cstdio>

struct CMusicManager {
    void* f0;
    void* f4;
    void* f8;
    void* fc;
    void* f10;
};

static void* model(CMusicManager* self)
{
    void* p;
    if ((p = self->f8) != 0) return p;
    if ((p = self->fc) != 0) return p;
    if ((p = self->f4) != 0) return p;
    if ((p = self->f10) != 0) return p;
    return self->f0;
}

int main()
{
    void* A = (void*)0x11110000;
    void* B = (void*)0x22220000;
    void* C = (void*)0x33330000;
    void* D = (void*)0x44440000;
    void* E = (void*)0x55550000;

    int ok = 1;

    // f8 non-null wins (priority 1)
    { CMusicManager m = {E,B,A,C,D}; ok &= (model(&m) == A); }
    // f8 null, fc wins (priority 2)
    { CMusicManager m = {E,B,0,C,D}; ok &= (model(&m) == C); }
    // f8,fc null, f4 wins (priority 3)
    { CMusicManager m = {E,B,0,0,D}; ok &= (model(&m) == B); }
    // f8,fc,f4 null, f10 wins (priority 4)
    { CMusicManager m = {E,0,0,0,D}; ok &= (model(&m) == D); }
    // all upper null, return f0
    { CMusicManager m = {E,0,0,0,0}; ok &= (model(&m) == E); }
    // everything null -> f0 null
    { CMusicManager m = {0,0,0,0,0}; ok &= (model(&m) == 0); }

    if (ok) printf("BEHAVIOUR_PASS_006bd760\n");
    else    printf("BEHAVIOUR_FAIL\n");
    return 0;
}