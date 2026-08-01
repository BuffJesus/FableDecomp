struct CCharStringData
{
    long m_begin;
    long m_end;
};

class CCharString
{
public:
    CCharStringData* m_data;

    long GetLength();
};

long CCharString::GetLength()
{
    CCharStringData* data = m_data;
    if (data == 0)
    {
        return 0;
    }
    return (data->m_end - data->m_begin) >> 1;
}