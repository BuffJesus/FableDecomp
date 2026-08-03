struct CDataInputStream
{
    virtual ~CDataInputStream();

    CDataInputStream();
};

struct CMemoryDataInputStream : CDataInputStream
{
    unsigned long m_field04;
    unsigned long m_field08;
    unsigned long m_field0c;
    unsigned long m_field10;
    unsigned long m_field14;
    unsigned long m_field18;

    CMemoryDataInputStream(void* buf, unsigned long size);
};

CMemoryDataInputStream::CMemoryDataInputStream(void* buf, unsigned long size)
    : CDataInputStream(),
      m_field08(size),
      m_field14(size),
      m_field04(0),
      m_field0c((unsigned long)buf),
      m_field10(0),
      m_field18((unsigned long)buf)
{
}
