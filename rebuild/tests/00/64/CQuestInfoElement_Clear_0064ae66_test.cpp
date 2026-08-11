#include <cstdio>

struct SubList
{
    void* m_00;
    void* m_04;
    void* m_08;
    void* m_0c;
    void* m_10;
    void PopEntry();
};

struct CQuestInfoElement
{
    char     m_pad[0x60];
    int      m_field60;
    char     m_pad2[0x04];
    SubList  m_list;
    void Clear();
};

// Simulated pop: walk m_10 one step toward m_00 (pointer array shrink).
void SubList::PopEntry()
{
    char** p = (char**)&m_10;
    *p = *p - 1;   // move the "end" pointer back by one element unit
}

void CQuestInfoElement::Clear()
{
    m_field60 = 0;
    SubList* sub = &m_list;
    while (sub->m_10 != sub->m_00)
        sub->PopEntry();
}

int main()
{
    CQuestInfoElement e;
    e.m_field60 = 12345;

    // set begin and end so end is 4 units above begin
    char* base = (char*)0x1000;
    e.m_list.m_00 = base;
    e.m_list.m_10 = base + 4;   // 4 pop steps needed (PopEntry subtracts 1 byte each)
    // to make it terminate we make PopEntry subtract 1 byte and gap=4 -> 4 iterations
    e.m_list.m_04 = 0;
    e.m_list.m_08 = 0;
    e.m_list.m_0c = 0;

    e.Clear();

    bool ok = (e.m_field60 == 0) && (e.m_list.m_10 == e.m_list.m_00);
    if (ok)
        printf("CQuestInfoElement_Clear_PASS\n");
    else
        printf("FAIL f60=%d end=%p beg=%p\n", e.m_field60, e.m_list.m_10, e.m_list.m_00);
    return ok ? 0 : 1;
}