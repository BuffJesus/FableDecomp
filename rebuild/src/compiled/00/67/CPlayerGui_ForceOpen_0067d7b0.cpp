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