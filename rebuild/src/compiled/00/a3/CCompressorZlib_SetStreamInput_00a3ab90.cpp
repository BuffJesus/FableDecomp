struct CCompressorZlib {
    char pad[0x1c];
    void* streamInput;   // +0x1c
    unsigned long size;  // +0x20
    int field24;         // +0x24
    int field28;         // +0x28
    int field2c;         // +0x2c
    void SetStreamInput(void* input, unsigned long size);
};

void CCompressorZlib::SetStreamInput(void* input, unsigned long sz)
{
    this->streamInput = input;
    this->field28 = 0;
    this->size = sz;
    this->field2c = 0;
}