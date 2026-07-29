// CFrontendGameComponent::SetMode @ 0x00495FE0.

#include <cstring>

class CFrontendModeList_00495fe0
{
public:
    void Refresh();
};

class CFrontendModePointer_00495fe0
{
    void* m_Data;

public:
    void Assign(const CFrontendModePointer_00495fe0*);
};

extern CFrontendModePointer_00495fe0 g_EmptyFrontendModePointer_00495fe0;

class CFrontendGameComponent_00495fe0
{
    unsigned char m_Pad0000[0x1c];
    long m_Mode;
    unsigned char m_Pad0020[0x148];
    long m_ResetValues[32];
    long m_Cursor;
    unsigned char m_Pad01ec[0x1c20];
    CFrontendModeList_00495fe0 m_ProfileList;
    unsigned char m_Pad1e0d[0x0b];
    long m_ProfileIndex;
    CFrontendModeList_00495fe0 m_SaveList;
    unsigned char m_Pad1e1d[0x0b];
    long m_SaveIndex;
    unsigned char m_Pad1e2c[0x14];
    CFrontendModePointer_00495fe0 m_ModePointer;
    long m_ModePointerEnabled;

    void PrepareModeTen();

public:
    void SetMode(long);
};

void CFrontendGameComponent_00495fe0::SetMode(long mode)
{
    m_Mode = mode;
    m_ModePointerEnabled = 1;
    std::memset(m_ResetValues, 0, sizeof(m_ResetValues));
    m_Cursor = 0;

    if (m_Mode == 10)
    {
        PrepareModeTen();
        return;
    }

    if (mode == 11)
    {
        m_SaveList.Refresh();
        m_ModePointer.Assign(&g_EmptyFrontendModePointer_00495fe0);
        m_SaveIndex = 0;
        return;
    }

    if (mode == 12)
    {
        m_ProfileList.Refresh();
        m_ProfileIndex = 0;
    }
}
