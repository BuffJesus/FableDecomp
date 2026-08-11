#include <stdio.h>
#include <stdlib.h>

struct CThingComponentSet {
    void*         m_data;
    unsigned char m_flag;
    ~CThingComponentSet();
};

CThingComponentSet::~CThingComponentSet()
{
    free(m_data);
    m_data = 0;
    m_flag = 0;
}

int main()
{
    CThingComponentSet s;
    s.m_data = malloc(64);
    s.m_flag = 1;
    s.~CThingComponentSet();

    if (s.m_data == 0 && s.m_flag == 0) {
        printf("DTOR_OK\n");
        return 0;
    }
    printf("DTOR_FAIL\n");
    return 1;
}