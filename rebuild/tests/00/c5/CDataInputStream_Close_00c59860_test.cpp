#include <cstdio>

struct CDataInputStream {
    char pad[0x18];
    unsigned int f18;
    unsigned int f1c;
    unsigned int f20;
    unsigned int f24;
    unsigned int f28;
    unsigned int f2c;
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

int main()
{
    CDataInputStream s;
    // prime with junk
    s.f18 = 0xdead; s.f1c = 0xbeef; s.f20 = 0x1234;
    s.f24 = 0x5678; s.f28 = 0x9abc; s.f2c = 0xdef0;
    s.Close();
    if (s.f18 == 0 && s.f1c == 0 && s.f20 == 0 &&
        s.f24 == 0 && s.f2c == 0 && s.f28 == 1) {
        printf("CLOSE_OK\n");
        return 0;
    }
    printf("CLOSE_BAD\n");
    return 1;
}