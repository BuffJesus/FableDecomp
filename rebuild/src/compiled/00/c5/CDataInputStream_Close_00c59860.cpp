struct CDataInputStream {
    char pad[0x18];
    unsigned int f18;  // 0x18
    unsigned int f1c;  // 0x1c
    unsigned int f20;  // 0x20
    unsigned int f24;  // 0x24
    unsigned int f28;  // 0x28
    unsigned int f2c;  // 0x2c
    void Close();
};

void CDataInputStream::Close()
{
    this->f18 = 0;
    this->f1c = 0;
    this->f20 = 0;
    this->f24 = 0;
    this->f2c = 0;
    this->f28 = 1;
}