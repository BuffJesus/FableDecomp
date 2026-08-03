
class CDataInputStream
{
public:
    void SetPosition(unsigned long pos);

    unsigned long m_unused0;   // +0x00
    unsigned long m_cursor;    // +0x04
    unsigned long m_unused1;   // +0x08
    unsigned long m_window;    // +0x0c
    unsigned long m_srcbase;   // +0x10
    unsigned long m_remaining; // +0x14
};

void CDataInputStream::SetPosition(unsigned long pos)
{
    if (m_window != 0 && pos >= m_srcbase && pos <= m_cursor + m_remaining)
    {
        m_remaining = (m_cursor + m_remaining) - pos;
        m_window = (m_window - m_cursor) + pos;
        m_cursor = pos;
    }
    else
    {
        m_window = 0;
        m_srcbase = 0;
        m_remaining = 0;
        m_cursor = pos;
    }
}