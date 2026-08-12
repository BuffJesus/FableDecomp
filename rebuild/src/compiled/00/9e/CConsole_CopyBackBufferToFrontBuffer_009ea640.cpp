struct CConBuf {
    void* vp;
    unsigned char b;
    int d;
    void Assign(CConBuf* src);
};
struct CConsole {
    char pad20[0x20];
    CConBuf front;
    char pad2c[4];
    CConBuf back;
    void CopyBackBufferToFrontBuffer();
};
void CConsole::CopyBackBufferToFrontBuffer()
{
    volatile CConsole* self = this;
    this->front.Assign(&this->back);
    self->front.b = self->back.b;
    self->front.d = self->back.d;
}