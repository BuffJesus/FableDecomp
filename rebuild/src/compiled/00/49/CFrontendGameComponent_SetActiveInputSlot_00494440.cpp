// Uncataloged CFrontendGameComponent input-slot selection helper
// @ 0x00494440.

class CFrontendGameComponent_00494440
{
    unsigned char m_Pad0000[0x28];
    long m_InputSlots[113];
    long* m_ActiveInputSlot;

public:
    void SetActiveInputSlot(long);
};

void CFrontendGameComponent_00494440::SetActiveInputSlot(long index)
{
    m_ActiveInputSlot = &m_InputSlots[index];
}
