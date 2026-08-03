#include <cstdio>

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

char *CStringTableArray::GetData()
{
    return m_data;
}

int main()
{
    unsigned char buf[64];
    for (int i = 0; i < 64; i++) buf[i] = (unsigned char)(i * 7 + 3);

    CDefStringTable table;
    table.m_vtbl = 0;
    table.m_field4 = 0;
    table.m_field8 = 0;
    table.m_strings.m_count = 16;
    table.m_strings.m_capacity = 16;
    table.m_strings.m_data = (char *)buf;

    unsigned long r0 = table.GetStringCRC(-1);
    unsigned long r1 = table.GetStringCRC(4);

    unsigned long expect1 = *(unsigned long *)(buf + 4);

    if (r0 == 0 && r1 == expect1)
    {
        printf("GETSTRINGCRC_OK\n");
    }
    else
    {
        printf("FAIL r0=%lu r1=%lu expect1=%lu\n", r0, r1, expect1);
    }

    return 0;
}