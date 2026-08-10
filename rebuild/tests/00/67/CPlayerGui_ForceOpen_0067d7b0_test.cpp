#include <cstdio>

struct CPlayerGui {
    int   f0;
    int   f4;
    float f8;
    char  fc;
    char  fd;
};

CPlayerGui* __fastcall ForceOpen(CPlayerGui* self)
{
    self->fd = 0;
    self->fc = 0;
    self->f4 = 0;
    self->f0 = 0;
    self->f8 = 1.0f;
    return self;
}

int main()
{
    CPlayerGui g;
    g.f0 = 7; g.f4 = 7; g.f8 = 99.0f; g.fc = 9; g.fd = 9;
    CPlayerGui* r = ForceOpen(&g);
    if (r == &g && g.f0 == 0 && g.f4 == 0 && g.fc == 0 && g.fd == 0 && g.f8 == 1.0f)
        printf("OK_0067d7b0\n");
    else
        printf("FAIL\n");
    return 0;
}