class CAIStateGroup_MinionBehaviourSad
{
public:
    char pad_0000[0x3c];
    unsigned char m_flag3c;
    unsigned char m_flag3d;
    unsigned char pad_3e[2];
    unsigned long m_field40;

    void InitLeaf();
};

void CAIStateGroup_MinionBehaviourSad::InitLeaf()
{
    m_flag3c = 0;
    m_flag3d = 0;
    m_field40 = 0;
}