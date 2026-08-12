#include <stdio.h>

static void* g_freed = (void*)0;

struct CPackedUIntArray {
    unsigned int* m_buffer;
    unsigned int  m_count;
    bool          m_flag;
    unsigned int  m_capacity;
    ~CPackedUIntArray();
};

void operator_delete_stub(void* p) { g_freed = p; }

CPackedUIntArray::~CPackedUIntArray()
{
    operator_delete_stub(m_buffer);
    m_buffer = 0;
    m_count = 0;
    m_flag = false;
    m_capacity = 0;
}

int main()
{
    unsigned int dummy[4] = {1,2,3,4};
    CPackedUIntArray a;
    a.m_buffer = dummy;
    a.m_count = 7;
    a.m_flag = true;
    a.m_capacity = 99;

    a.~CPackedUIntArray();

    bool ok = (g_freed == (void*)dummy)
           && (a.m_buffer == 0)
           && (a.m_count == 0)
           && (a.m_flag == false)
           && (a.m_capacity == 0);

    if (ok) printf("DTOR_OK\n");
    else    printf("DTOR_FAIL\n");
    return ok ? 0 : 1;
}