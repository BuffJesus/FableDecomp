#include <cstdio>

struct CCompressorZlib {
    void* m_pad0;
    void* m_input;
    unsigned long m_pos;
    unsigned long m_length;
    bool SetStreamInput(void* input, unsigned long length);
};

bool CCompressorZlib::SetStreamInput(void* input, unsigned long length)
{
    m_input = input;
    m_pos = 0;
    m_length = length;
    return true;
}

int main()
{
    CCompressorZlib c;
    c.m_pad0 = (void*)0xDEADBEEF;
    c.m_input = 0;
    c.m_pos = 99;
    c.m_length = 0;

    int buf = 42;
    bool r = c.SetStreamInput(&buf, 128);

    if (r && c.m_input == &buf && c.m_pos == 0 && c.m_length == 128
        && c.m_pad0 == (void*)0xDEADBEEF) {
        printf("SETSTREAMINPUT_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}