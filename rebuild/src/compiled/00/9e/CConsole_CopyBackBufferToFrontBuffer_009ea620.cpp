// CConsole::CopyBackBufferToFrontBuffer  retail 0x009ea620

struct ConBuffer {
    void*         data;    // +0x00
    unsigned char flag;    // +0x04
    int           extra;   // +0x08
    int           pad;     // +0x0C
    // member at 0x99efb0: copies contents of another buffer
    void CopyContents(ConBuffer* src);
};

struct CConsole {
    char      pad0[0x20];  // +0x00
    ConBuffer back;        // +0x20
    ConBuffer front;       // +0x30
    void CopyBackBufferToFrontBuffer();
};

void CConsole::CopyBackBufferToFrontBuffer()
{
    this->front.CopyContents(&this->back);
    this->front.flag = this->back.flag;
    int* pd = &this->front.extra;
    int* ps = &this->back.extra;
    *pd = *ps;
}