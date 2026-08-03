struct CStringTableArray
{
    unsigned long m_count;
    unsigned long m_capacity;
    char *m_data;

    char *GetData();
};

struct CDefStringTable
{
    void *m_vtbl;
    unsigned long m_field4;
    unsigned long m_field8;
    CStringTableArray m_strings;

    unsigned long GetStringCRC(long index);
};

unsigned long CDefStringTable::GetStringCRC(long index)
{
    if (index == -1)
        return 0;
    char *p = m_strings.GetData();
    p += index;
    return *(unsigned long *)p;
}