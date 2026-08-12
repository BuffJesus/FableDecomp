struct CTCVillage {
    char pad[0xb0];
    char m_b0;
    char m_b1;
    char ComputeB0();
    char ComputeB1();
    void ResetCrier();
};

void CTCVillage::ResetCrier()
{
    this->m_b0 = this->ComputeB0();
    this->m_b1 = this->ComputeB1();
}