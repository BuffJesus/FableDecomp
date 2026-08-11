struct CWorld {
    char pad[0x10];
    unsigned int m_val;
    int DrawGetEnvironment(unsigned int threshold);
};

int CWorld::DrawGetEnvironment(unsigned int threshold)
{
    return this->m_val >= threshold ? 1 : 0;
}