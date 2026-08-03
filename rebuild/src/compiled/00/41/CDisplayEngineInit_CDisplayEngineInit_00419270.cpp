struct DisplayEngineState {
    int f00;
    int f04;
    int f08;
    int f0c;
    int f10;
    int f14;
    int f18;
    int handle;   // +0x1c
    int f20;
    int f24;
};

void* __fastcall CDisplayEngineInit(DisplayEngineState* self)
{
    self->handle = -1;
    self->f00 = 0;
    self->f04 = 0;
    self->f08 = 0;
    self->f0c = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f18 = 0;
    self->f20 = 0;
    return self;
}