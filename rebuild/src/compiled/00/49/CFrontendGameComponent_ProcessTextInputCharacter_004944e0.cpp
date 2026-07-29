// Uncataloged CFrontendGameComponent text-input character helper
// @ 0x004944E0.

class CFrontendGameComponent_004944e0
{
    unsigned char m_Pad0000[0x168];
    char m_TextInput[128];
    long m_TextInputLength;

public:
    void ProcessTextInputCharacter(unsigned short);
};

void CFrontendGameComponent_004944e0::ProcessTextInputCharacter(
    unsigned short character)
{
    if (character == 8)
    {
        if (m_TextInputLength > 0)
        {
            --m_TextInputLength;
            m_TextInput[m_TextInputLength] = '\0';
        }
    }
    else if (m_TextInputLength < 127)
    {
        m_TextInput[m_TextInputLength] = static_cast<char>(character);
        ++m_TextInputLength;
    }
}
