struct CCompressorZlib {
    void* m_pad0;      // +0
    void* m_input;     // +4
    unsigned long m_pos;   // +8
    unsigned long m_length;// +0xc

    bool SetStreamInput(void* input, unsigned long length);
};

bool CCompressorZlib::SetStreamInput(void* input, unsigned long length)
{
    m_input = input;
    m_pos = 0;
    m_length = length;
    return true;
}