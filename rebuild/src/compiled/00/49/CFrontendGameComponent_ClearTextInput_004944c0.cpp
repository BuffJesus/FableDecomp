// Uncataloged CFrontendGameComponent text-input reset helper @ 0x004944C0.

#include <cstring>

class CFrontendGameComponent_004944c0
{
    unsigned char m_Pad0000[0x168];
    char m_TextInput[128];
    long m_TextInputLength;

public:
    void ClearTextInput();
};

void CFrontendGameComponent_004944c0::ClearTextInput()
{
    std::memset(m_TextInput, 0, sizeof(m_TextInput));
    m_TextInputLength = 0;
}
